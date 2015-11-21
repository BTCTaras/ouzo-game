#include "graphics.h"

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <stdio.h>

const unsigned int CGraphics::VERT_ATTRIB_POS = 0;
const unsigned int CGraphics::VERT_ATTRIB_TEX_COORDS = 1;

void CGraphics::Init() {
  glGenVertexArrays(1, &m_vao);
  glEnable(GL_TEXTURE_2D);

  m_defaultProgram.reset(new CProgram);

  S_CShader shaders[] = {
    S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/diffuse.vsh")),
    S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/diffuse.fsh"))
  };

  m_defaultProgram->LoadFromShaders(2, shaders);
}

CGraphics::~CGraphics() {
  if (m_vao > 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}

S_CProgram CGraphics::GetDefaultProgram() {
  return m_defaultProgram;
}

void CGraphics::Begin(mvp_matrix_t &mvp, S_CProgram program) {
  if (m_vao == 0) {
    fprintf(stderr, "WARNING: CGraphics has no VAO!!\n");
  }

  S_CProgram prog = (program != nullptr) ? program : m_defaultProgram;
  prog->Use();

  glm::mat4 mvpMat = mvp.projection * mvp.view * mvp.model;

  unsigned int mvpLoc = prog->GetUniformLocation("u_MVPMatrix");
  glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

  unsigned int texLoc = prog->GetUniformLocation("u_Texture");
  glUniform1i(texLoc, 0);

  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(CGraphics::VERT_ATTRIB_POS);
  glVertexAttribPointer(CGraphics::VERT_ATTRIB_POS,
                        3, GL_FLOAT, GL_FALSE,
                        sizeof(vertex_t), (void*)offsetof(vertex_t, x));

  glEnableVertexAttribArray(CGraphics::VERT_ATTRIB_TEX_COORDS);
  glVertexAttribPointer(CGraphics::VERT_ATTRIB_TEX_COORDS,
                        2, GL_FLOAT, GL_FALSE,
                        sizeof(vertex_t), (void*)offsetof(vertex_t, u));
}

void CGraphics::End() {
  glDisableVertexAttribArray(CGraphics::VERT_ATTRIB_TEX_COORDS);
  glDisableVertexAttribArray(CGraphics::VERT_ATTRIB_POS);

  glUseProgram(0);
}
