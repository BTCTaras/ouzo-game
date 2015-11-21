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

  float x = 0;
  float y = 0;

  for (size_t i = 0; i < m_text.length(); ++i) {
    unsigned long c = m_text[i];
    if (c == (unsigned long) ' ') {
      x += m_size / 4; // TODO: Un-hardcode this.
    }

    glyph_data_t *gdata = m_font->GetGlyphData(c);

    float x2 = x + gdata->bitmapLeft;
    float y2 = -y - gdata->bitmapTop;
    float w = gdata->width;
    float h = gdata->height;

    float u1 = gdata->u1;
    float v1 = gdata->v2 - gdata->v1;
    float u2 = gdata->u2;
    float v2 = v1 - gdata->v2;

    // Triangle 1
    vertices.push_back({ x2,      h + y2,      0.0f, u1, v1 });
    vertices.push_back({ x2,      y2,           0.0f, u1, v2 });
    vertices.push_back({ x2 + w,  h + y2,      0.0f, u2, v1 });

    // Triangle 2
    vertices.push_back({ x2,      y2,           0.0f, u1, v2 });
    vertices.push_back({ x2 + w,  y2,           0.0f, u2, v2 });
    vertices.push_back({ x2 + w,  h + y2,      0.0f, u2, v1 });

    x += gdata->advanceX >> 6;
    y += gdata->advanceY >> 6;
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

  //glBindTexture(GL_TEXTURE_2D, 1); // Uses the button texture to make wireframe visible.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Turn on wireframe.
  glDrawArrays(GL_TRIANGLES, 0, m_fontBufferSize); // Render text.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Turn off wireframe.

  GFX->End();
}

unsigned int CText::GetBuffer() {
  return m_fontBuffer;
}
