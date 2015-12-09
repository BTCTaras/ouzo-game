#include "graphics_common/graphics.hpp"

#include "graphics_common/shader.hpp"
#include "graphics_common/texture.hpp"
#include "graphics_common/atlas_factory.hpp"
#include "graphics_common/buffer.hpp"
#include "graphics_common/draw_attribs.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>

#ifdef OUZO_DEBUG

// Not sure why this hackiness is needed but it is.
#ifdef _WIN32
void __stdcall GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#endif

#ifdef __APPLE__
void GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#endif 

#ifdef __UNIX__
void GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#endif

#endif

void CGLGraphics::Init(SDL_Window *window) {
	m_lastActiveTex = 0xFFFFFFFF;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

	// Create an OpenGL 3.3 context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Use the core profile.
#ifdef OUZO_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_context = SDL_GL_CreateContext(window); // Create an OpenGL context.
	SDL_GL_MakeCurrent(window, m_context); // Make sure our OpenGL calls are directed to this context.

	m_window = window;

	static bool inittedGLEW = false;

	// Initialise GLEW if needed.
	if (!inittedGLEW) {
		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialise GLEW!!\n");
			exit(2);
			return;
		}

		inittedGLEW = true;
	}

	glGenVertexArrays(1, &m_vao);

	S_CShader defaultVert = this->CreateShader(ShaderType::GFX_VERTEX_SHADER, "assets/shaders/diffuse");
	S_CShader defaultFrag = this->CreateShader(ShaderType::GFX_FRAGMENT_SHADER, "assets/shaders/diffuse");

	// Load the default program.
	S_CShader shaders[] = { defaultVert, defaultFrag };
	m_defaultProgram = this->CreateProgram(2, shaders);

	const GLubyte *glVersion = glGetString(GL_VERSION);
	printf("Graphics Driver: %s\n", glVersion);

	// Ensures that objects behind objects are occluded.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint textureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);

	printf("OpenGL max texture size is %dx%d\n", textureSize, textureSize);

	m_glMaxTextureSize = textureSize;

#ifdef OUZO_DEBUG
	if (glewIsSupported("GL_ARB_debug_output")) {
		printf("GL_ARB_debug_output is supported!! Enabling debug output...\n");
		glDebugMessageCallbackARB(GL_DebugCallback, this);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 0, NULL, GL_FALSE);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
#endif

	S_CMatrix mat = this->CreateIdentityMatrix();
	m_defaultInstData.AddInstance(mat);
}

#ifdef OUZO_DEBUG
#ifdef _WIN32
void __stdcall GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
#endif
    
#ifdef __APPLE__
void GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)  {
#endif
    
#ifdef __UNIX__
void GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam) {
#endif
	const char *typeName;
	switch (type) {
	case GL_DEBUG_TYPE_ERROR_ARB:
		typeName = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: // behaviour*
		typeName = "DEPRECATED_BEHAVIOUR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: // behaviour*
		typeName = "UNDEFINED_BEHAVIOUR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB:
		typeName = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:
		typeName = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER_ARB:
	default:
		typeName = "OTHER";
		break;
	}

	const char *severityName;
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH_ARB:
		severityName = "HIGH";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:
		severityName = "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_LOW_ARB:
		severityName = "LOW";
		break;
	default:
		severityName = "N/A";
		break;
	}

	const char *sourceName;
	switch (source) {
	case GL_DEBUG_SOURCE_API_ARB:
		sourceName = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
		sourceName = "WINDOW_SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
		sourceName = "SHADER_COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
		sourceName = "THIRD_PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION_ARB:
		sourceName = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER_ARB:
	default:
		sourceName = "OTHER";
		break;
	}

	printf("[%s] Driver Message from %s (%s): %s\n", severityName, sourceName, typeName, message);
}
#endif

CGLGraphics::~CGLGraphics() {
	if (m_vao > 0) {
		glDeleteVertexArrays(1, &m_vao);
	}

	SDL_GL_DeleteContext(m_context);
}

void CGLGraphics::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	glViewport(x, y, w, h);
}

S_CProgram CGLGraphics::GetDefaultProgram() {
	return m_defaultProgram;
}

S_CMatrix CGLGraphics::CreateIdentityMatrix() {
	return S_CGLMatrix(new CGLMatrix);
}

S_CDrawAttribs CGLGraphics::CreateDrawAttribs() {
	return S_CGLDrawAttribs(new CGLDrawAttribs);
}

S_CDrawAttribs CGLGraphics::CreateDrawAttribs(S_CBuffer buffer) {
	S_CGLDrawAttribs attribs = S_CGLDrawAttribs(new CGLDrawAttribs);
	attribs->SetSource(AttribType::POSITION, buffer, offsetof(vertex_t, x));
	attribs->SetSource(AttribType::TEX_COORDS, buffer, offsetof(vertex_t, u));
	attribs->SetSource(AttribType::NORMAL, buffer, offsetof(vertex_t, nx));
	m_defaultInstData.LoadInto(attribs);
	return attribs;
}

S_CTexture CGLGraphics::CreateTexture(const char *file) {
	S_CGLTexture tex(new CGLTexture);

	if (file != NULL) {
		tex->LoadFromFile(file);
	}

	return tex;
}

static const char *GL_SHADER_FILE_EXT = "glsl";

S_CShader CGLGraphics::CreateShader(ShaderType type, const char *file) {
	std::ostringstream str;
	std::string typeExt =
		type == ShaderType::GFX_VERTEX_SHADER ? "vert" : "frag";

	str << file << "." << typeExt << "." << GL_SHADER_FILE_EXT;
	std::string filename = str.str();

	S_CGLShader shader = S_CGLShader(new CGLShader(type, filename.c_str()));
	return shader;
}

S_CProgram CGLGraphics::CreateProgram(size_t count, S_CShader *shaders) {
	S_CGLProgram program(new CGLProgram(count, shaders));
	return program;
}

S_CAtlasFactory CGLGraphics::CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) {
	S_CGLAtlasFactory factory(new CGLAtlasFactory(width, height, channels));
	return factory;
}

S_CBuffer CGLGraphics::CreateBuffer(BufferType type, BufferStorageType storageType) {
	S_CGLBuffer buffer(new CGLBuffer(type, storageType));
	return buffer;
}

unsigned int CGLGraphics::GetMaxTextureSize() {
	return m_glMaxTextureSize;
}

int CGLGraphics::GetMaxInstanceCount() {
	return 128;
}

void CGLGraphics::BeginScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGLGraphics::SetDrawAttributes(S_CDrawAttribs attribs) {
	S_CGLDrawAttribs gl_attribs = std::static_pointer_cast<CGLDrawAttribs>(attribs);
	glBindVertexArray(gl_attribs->GetOpenGLHandle());
}

void CGLGraphics::SetDrawProgram(S_CProgram program) {
	S_CGLProgram gl_prog = std::static_pointer_cast<CGLProgram>(program);
	m_drawProgram = gl_prog;
}

void CGLGraphics::Draw(PrimitiveType primitive, S_CBuffer elementBuffer) {
	if (m_currentBuffer == nullptr) {
		fprintf(stderr, "Tried to draw with nullptr buffer!! Did you forget to call SetDrawBuffer?\n");
		return;
	}

	S_CGLProgram gl_prog = std::static_pointer_cast<CGLProgram>(m_drawProgram);
	gl_prog->Use();

	GLenum gl_primitive = CGLGraphics::GetOpenGLPrimitiveTypeEnum(primitive);

	if (elementBuffer != nullptr) {
		S_CGLBuffer gl_elementBuffer = std::static_pointer_cast<CGLBuffer>(elementBuffer);
		// We have an element buffer, use glDrawElements
		// Specify what element buffer to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_elementBuffer->GetOpenGLHandle());
		glDrawElements(
			gl_primitive,
			elementBuffer->GetElementCount(), // Amount of elements = elem buffer size / elem size
			GL_UNSIGNED_SHORT, // Each element is an unsigned short
			NULL // We've already bound our indices as a buffer
		);
	} else {
		// We have no element buffer, draw regularily
		glDrawArrays(
			gl_primitive,
			0, // The first vertex is at 0
			m_currentBuffer->GetElementCount() // Amount of vertices = vertex buffer size / vertex size
		);
	}
}

void CGLGraphics::DrawInstanced(PrimitiveType primitive, size_t instanceCount, S_CBuffer elementBuffer) {
    if (m_currentBuffer == nullptr) {
        fprintf(stderr, "Tried to draw instanced with nullptr buffer!! Did you forget to call SetDrawBuffer?\n");
		return;
    }

    S_CGLProgram gl_prog = std::static_pointer_cast<CGLProgram>(m_drawProgram);
    glUseProgram(gl_prog->GetOpenGLHandle());

    GLenum gl_primitive = CGLGraphics::GetOpenGLPrimitiveTypeEnum(primitive);

    if (elementBuffer != nullptr) {
        S_CGLBuffer gl_elementBuffer = std::static_pointer_cast<CGLBuffer>(elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_elementBuffer->GetOpenGLHandle());

		glDrawElementsInstanced(
			gl_primitive,
			instanceCount,
			GL_UNSIGNED_SHORT,
			NULL,
			gl_elementBuffer->GetElementCount()
		);
	} else {
		glDrawArraysInstanced(
			gl_primitive,
			0,
			m_currentBuffer->GetElementCount(),
			instanceCount
		);
	}
}

void CGLGraphics::SetDrawTexture(S_CTexture tex, unsigned int slot) {
	S_CGLTexture gl_tex = std::static_pointer_cast<CGLTexture>(tex);

	unsigned int texSlot = GL_TEXTURE0 + slot;
	if (m_lastActiveTex != texSlot) {
		glActiveTexture(texSlot); // slot must be offset by GL_TEXTURE0
		m_lastActiveTex = texSlot;
	}

	if (gl_tex != nullptr) {
		glBindTexture(GL_TEXTURE_2D, gl_tex->GetOpenGLHandle());
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CGLGraphics::SetDrawTransform(mvp_matrix_t &mvp) {
	S_CMatrix fullmvp = mvp.projection * mvp.view * mvp.model;
	S_CMatrix vp = mvp.projection * mvp.view;

	m_drawProgram->SetUniform(
		ShaderUniformType::GFX_MAT4x4F,
		"u_ModelMatrix",
		mvp.model->ValuePointer()
	);

	m_drawProgram->SetUniform(
		ShaderUniformType::GFX_MAT4x4F,
		"u_ViewMatrix",
		mvp.view->ValuePointer()
	);
	
	m_drawProgram->SetUniform(
		ShaderUniformType::GFX_MAT4x4F,
		"u_VPMatrix",
		vp->ValuePointer()
	);

	m_drawProgram->SetUniform(
		ShaderUniformType::GFX_MAT4x4F,
		"u_ProjectionMatrix",
		mvp.projection->ValuePointer()
	);

	m_drawProgram->SetUniform(
		ShaderUniformType::GFX_MAT4x4F,
		"u_MVPMatrix",
		fullmvp->ValuePointer()
	);
}

void CGLGraphics::SetDrawBuffer(S_CBuffer buffer) {
	m_currentBuffer = buffer;
}

unsigned int CGLGraphics::GetOpenGLPrimitiveTypeEnum(PrimitiveType primitive) {
	switch (primitive) {
	case PrimitiveType::GFX_POINTS:
		return GL_POINTS;
	case PrimitiveType::GFX_LINE_STRIP:
		return GL_LINE_STRIP;
	case PrimitiveType::GFX_LINE_LOOP:
		return GL_LINE_LOOP;
	case PrimitiveType::GFX_LINES:
		return GL_LINES;
	case PrimitiveType::GFX_TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case PrimitiveType::GFX_TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	case PrimitiveType::GFX_TRIANGLES:
		return GL_TRIANGLES;
	default:
		return GL_FALSE;
	}
}

void CGLGraphics::EndScene() {
	SDL_GL_SwapWindow(m_window);
}
