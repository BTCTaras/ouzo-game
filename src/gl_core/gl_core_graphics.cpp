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

void CGLGraphics::Init(SDL_Window *window) {
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

  // Create an OpenGL 3.3 context.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  // Use the core profile.
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

  S_CShader defaultVert = this->CreateShader(ShaderType::VERTEX_SHADER, "assets/shaders/diffuse");
  S_CShader defaultFrag = this->CreateShader(ShaderType::FRAGMENT_SHADER, "assets/shaders/diffuse");

  // Load the default program.
  S_CShader shaders[] = { defaultVert, defaultFrag };
  m_defaultProgram = this->CreateProgram(2, shaders);

  const GLubyte *glVersion = glGetString(GL_VERSION);
  printf("Graphics Driver: %s\n", glVersion);

  // Make sure transparent objects are rendered properly.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Ensures that objects behind objects are occluded.
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  GLint textureSize;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);

  printf("OpenGL max texture size is %dx%d\n", textureSize, textureSize);

  m_glMaxTextureSize = textureSize;
}

CGLGraphics::~CGLGraphics() {
  if (m_vao > 0) {
    glDeleteVertexArrays(1, &m_vao);
  }

  SDL_GL_DeleteContext(m_context);
}

S_CProgram CGLGraphics::GetDefaultProgram() {
  return m_defaultProgram;
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
    type == ShaderType::VERTEX_SHADER ? "vert" : "frag";

  str << file << "." << typeExt << "." << GL_SHADER_FILE_EXT;
  std::string filename = str.str();

  GLenum glType;
  switch (type) {
    case VERTEX_SHADER:
      glType = GL_VERTEX_SHADER;
      break;

    case FRAGMENT_SHADER:
      glType = GL_FRAGMENT_SHADER;
      break;

    default:
      glType = GL_VERTEX_SHADER;
      break;
  }

  S_CGLShader shader = S_CGLShader(new CGLShader(glType, filename.c_str()));
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

void CGLGraphics::Begin(mvp_matrix_t &mvp, S_CProgram program) {
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
  glm::mat4 mvpMat = mvp.projection * mvp.view * mvp.model;

  unsigned int mvpLoc = prog->GetUniformLocation("u_MVPMatrix", true);

  // upload our MVP matrix
  glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

  unsigned int texLoc = prog->GetUniformLocation("u_Texture", true);
  glUniform1i(texLoc, 0); // we wanna use the texture in slot 0 (GL_TEXTURE0)

  // Specify where to store our vertex attributes
  glBindVertexArray(m_vao);

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

void CGLGraphics::End() {
  // idk why this is necessary but it is
  glDisableVertexAttribArray(CGLGraphics::VERT_ATTRIB_TEX_COORDS);
  glDisableVertexAttribArray(CGLGraphics::VERT_ATTRIB_POS);
}

void CGLGraphics::EndScene() {
  SDL_GL_SwapWindow(m_window);
}