#pragma once

#include "graphics.h"
#include "shader.h"
#include "texture.h"
#include "font.h"

#include <string>
#include <memory>

class CText {
public:
  CText();
  CText(CFont *font, unsigned int size, const std::u32string &text);
  ~CText();

  void SetColour(colour_t colour);
  void InitText(CFont *font, unsigned int size, const std::u32string &text);

  void SetText(const std::u32string &text);

  void Render(mvp_matrix_t &mvp);
  unsigned int GetBuffer();

private:
  void CreateTextBuffer();

  static S_CProgram s_fontProgram;

  std::u32string m_text;
  unsigned int m_fontBuffer;
  unsigned int m_fontBufferSize;
  unsigned int m_size;

  colour_t m_colour;

  CFont *m_font;
};

typedef std::shared_ptr<CFont> S_CFont;
