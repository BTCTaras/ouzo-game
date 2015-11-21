#include "text.h"

#include <GL/glew.h>

#include "shader.h"
#include "graphics.h"
#include "game.h"
#include "font.h"
#include "texture.h"

#include <vector>

S_CProgram CText::s_fontProgram;

CText::CText() {
  m_colour = { 1.0f, 1.0f, 1.0f }; // Start white
}

CText::CText(CFont *font, unsigned int size, const std::u32string &text)
  : m_fontBuffer(0)
{
  this->InitText(font, size, text);
}

void CText::InitText(CFont *font, unsigned int size, const std::u32string &text) {
  if (s_fontProgram == nullptr) {
    S_CShader fontShaders[] = {
      S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/uitext.vsh")),
      S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/uitext.fsh")),
    };

    s_fontProgram.reset(new CProgram(2, fontShaders));
  }

  if (m_fontBuffer == 0) {
    glGenBuffers(1, &m_fontBuffer);
  }

  m_size = size;
  m_font = font;
  m_text = text;

  this->CreateTextBuffer();
}

CText::~CText() {
  if (m_fontBuffer > 0) {
    glDeleteBuffers(1, &m_fontBuffer);
  }
}

void CText::CreateTextBuffer() {
  std::vector<vertex_t> vertices;

  for (size_t i = 0; i < m_text.length(); ++i) {
    float x1 = i * m_size;
    float y1 = 0;
    float x2 = i * m_size + m_size;
    float y2 = m_size;

    unsigned long c = m_text[i];
    glyph_data_t *gdata = m_font->GetGlyphData(c);

    float u1 = gdata->u1;
    float v1 = gdata->v1;
    float u2 = gdata->u2;
    float v2 = gdata->v2;

    // Triangle 1
    vertices.push_back({ x1, y1, 0.0f, u1, v1 });
    vertices.push_back({ x1, y2, 0.0f, u1, v2 });
    vertices.push_back({ x2, y1, 0.0f, u2, v1 });

    // Triangle 2
    vertices.push_back({ x1, y2, 0.0f, u1, v2 });
    vertices.push_back({ x2, y2, 0.0f, u2, v2 });
    vertices.push_back({ x2, y1, 0.0f, u2, v1 });
  }

  m_fontBufferSize = vertices.size() * sizeof(vertex_t);

  glBindBuffer(GL_ARRAY_BUFFER, m_fontBuffer);
  glBufferData(GL_ARRAY_BUFFER, m_fontBufferSize, &vertices[0], GL_STATIC_DRAW);
}

void CText::SetColour(colour_t colour) {
  m_colour = colour;
}

void CText::Render(mvp_matrix_t &mvp) {
  S_CTexture atlas = m_font->GetAtlas(m_size);

  if (atlas == nullptr) {
    atlas = m_font->CreateAtlas(m_size);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_fontBuffer);

  GFX->Begin(mvp, s_fontProgram);
  glUniform3f(s_fontProgram->GetUniformLocation("u_FontColour"), m_colour.r, m_colour.g, m_colour.b);
  atlas->Use();
  glDrawArrays(GL_TRIANGLES, 0, m_fontBufferSize);
  GFX->End();
}

unsigned int CText::GetBuffer() {
  return m_fontBuffer;
}
