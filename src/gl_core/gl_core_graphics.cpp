#include "graphics.hpp"

#include "shader.hpp"
#include "texture.hpp"
#include "atlas_factory.hpp"
#include "buffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include <SDL2/SDL.h>

const unsigned int CGLGraphics::VERT_ATTRIB_POS = 0;
const unsigned int CGLGraphics::VERT_ATTRIB_TEX_COORDS = 1;

#ifdef OUZO_DEBUG

// Not sure why this hackiness is needed but it is.
#ifdef _WIN32
void __stdcall GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#else
void GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#endif

#endif

void CGLGraphics::Init(SDL_Window *window) {
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
	if (GL_ARB_debug_output) {
		printf("GL_ARB_debug_output is supported!! Enabling debug output...\n");
		glDebugMessageCallbackARB(GL_DebugCallback, this);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 0, NULL, GL_FALSE);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	}
#endif
}

#ifdef OUZO_DEBUG
#ifdef _WIN32
void __stdcall GL_DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
#else
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

void CGLGraphics::BeginScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CGLGraphics::Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program) {
	// OpenGL handles are 0 when uninitialised
	if (m_vao == 0) {
		fprintf(stderr, "WARNING: CGraphics has no VAO!!\n");
	}

	// if program points to 0, fall back to the default (diffuse) program.
	S_CGLProgram prog = std::static_pointer_cast<CGLProgram>(
		(program != nullptr) ? program : m_defaultProgram
		);
	prog->Use();

	// compute the MVP matrix
	S_CGLMatrix mvpMat = std::static_pointer_cast<CGLMatrix>(mvp.projection * mvp.view * mvp.model);
	unsigned int mvpLoc = prog->GetUniformLocation("u_MVPMatrix", true);

	// upload our MVP matrix
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvpMat->ValuePointer());

	unsigned int texLoc = prog->GetUniformLocation("u_Texture", true);
	glUniform1i(texLoc, 0); // we wanna use the texture in slot 0 (GL_TEXTURE0)

	// Specify where to store our vertex attributes
	glBindVertexArray(m_vao);

	// Specify where to find our data
	S_CGLBuffer gl_buffer = std::static_pointer_cast<CGLBuffer>(vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gl_buffer->GetOpenGLHandle());
	m_currentBuffer = vertexBuffer; // For use in CGLGraphics::Draw.

	// Enable the vertex attrib array idk why but you gotta do this
	glEnableVertexAttribArray(CGLGraphics::VERT_ATTRIB_POS);

	// Specify where to find our vertex position data
	glVertexAttribPointer(CGLGraphics::VERT_ATTRIB_POS,
		3,          // we want 3 floats per position
		GL_FLOAT,   // we wanna upload floats
		GL_FALSE,   // don't normalise our data
		sizeof(vertex_t), // position data occurs every sizeof(vertex_t) bytes
		(void*)offsetof(vertex_t, x) // position data is offsetof(x) bytes into each buffer element
		);

	// Ditto
	glEnableVertexAttribArray(CGLGraphics::VERT_ATTRIB_TEX_COORDS);

	// Specify where to find our UV data
	glVertexAttribPointer(CGLGraphics::VERT_ATTRIB_TEX_COORDS,
		2,          // we want 2 floats per tex coord
		GL_FLOAT,   // we wanna upload floats
		GL_FALSE,   // don't normalise our data
		sizeof(vertex_t), // texture coords occur every sizeof(vertex_t) bytes
		(void*)offsetof(vertex_t, u) // tex coord data is offsetof(u) bytes into each buffer element
		);
}

void CGLGraphics::Draw(PrimitiveType primitive, S_CBuffer elementBuffer) {
	GLenum gl_primitive = CGLGraphics::GetOpenGLPrimitiveTypeEnum(primitive);

	if (elementBuffer != nullptr) {
		S_CGLBuffer gl_elementBuffer = std::static_pointer_cast<CGLBuffer>(elementBuffer);
		// We have an element buffer, use glDrawElements
		// Specify what element buffer to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_elementBuffer->GetOpenGLHandle());
		glDrawElements(
			gl_primitive,
			elementBuffer->GetSize() / sizeof(unsigned short), // Amount of elements = elem buffer size / elem size
			GL_UNSIGNED_SHORT, // Each element is an unsigned short
			NULL // We've already bound our indices as a buffer
			);
	}
	else {
		// We have no element buffer, draw regularily
		glDrawArrays(
			gl_primitive,
			0, // The first vertex is at 0
			m_currentBuffer->GetSize() / sizeof(vertex_t) // Amount of vertices = vertex buffer size / vertex size
			);
	}
}

void CGLGraphics::SetTexture(S_CTexture tex, unsigned int slot) {
	S_CGLTexture gl_tex = std::static_pointer_cast<CGLTexture>(tex);
	glActiveTexture(GL_TEXTURE0 + slot); // slot must be offset by GL_TEXTURE0

	if (gl_tex != nullptr) {
		glBindTexture(GL_TEXTURE_2D, gl_tex->GetOpenGLHandle());
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
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

void CGLGraphics::End() {
	// idk why this is necessary but it is
	glDisableVertexAttribArray(CGLGraphics::VERT_ATTRIB_TEX_COORDS);
	glDisableVertexAttribArray(CGLGraphics::VERT_ATTRIB_POS);

	m_currentBuffer = nullptr;
}

void CGLGraphics::EndScene() {
	SDL_GL_SwapWindow(m_window);
}
