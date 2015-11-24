#include "graphics.hpp"

#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <stdio.h>

const unsigned int CGLGraphics::VERT_ATTRIB_POS = 0;
const unsigned int CGLGraphics::VERT_ATTRIB_TEX_COORDS = 1;

void CGLGraphics::Init() {
  glGenVertexArrays(1, &m_vao);

  // Load the default program.
  S_CShader shaders[] = {
    S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/diffuse.vsh")),
    S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/diffuse.fsh"))
  };

  m_defaultProgram.reset(new CProgram(2, shaders));

  // Make sure transparent objects are rendered properly.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Ensures that objects behind objects are occluded.
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  GLint textureSize;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureSize);

  printf("OpenGL max texture size is %dx%d\n", textureSize, textureSize);
}

CGLGraphics::~CGLGraphics() {
  if (m_vao > 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}

S_CProgram CGLGraphics::GetDefaultProgram() {
  return m_defaultProgram;
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
  S_CProgram prog = (program != nullptr) ? program : m_defaultProgram;
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

}
