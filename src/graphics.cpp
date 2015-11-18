#include "graphics.h"

#include <GL/glew.h>
#include <stdio.h>

const unsigned int CGraphics::VERT_ATTRIB_POS = 0;
const unsigned int CGraphics::VERT_ATTRIB_TEX_COORDS = 1;

void CGraphics::Init() {
  glGenVertexArrays(1, &m_vao);
}

CGraphics::~CGraphics() {
  if (m_vao > 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}

void CGraphics::Begin() {
  if (m_vao == 0) {
    fprintf(stderr, "WARNING: CGraphics has no VAO!!\n");
  }

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
}
