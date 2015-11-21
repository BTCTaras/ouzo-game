#pragma once

#include "texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class CFont {
public:
  ///
  /// See LoadFromFile for specifics.
  ///
  CFont(const char *file, unsigned int index = 0);
  ~CFont();

  ///
  /// Initialises this font from a file.
  /// \param[in]  file  The file to load the font from.
  /// \param[in]  index When using a format that supports multiple fonts in one, this specifies which font to load.
  //
  void LoadFromFile(const char *file, unsigned int index = 0);

  ///
  /// Creates a 3D texture of characters for use in OpenGL.
  /// \param[out] tex       The final texture.
  /// \param[in]  size      The size of each glyph in pixels.
  /// \param[in]  charCount The amount of characters contained in chars.
  /// \param[in]  chars     The chars to be rendered as glyphs.
  ///
  void CreateGlyphTexture(CTexture *tex, unsigned int size, size_t charCount, unsigned long *chars);

private:
  FT_Face m_fontFace;
  CTexture m_glyphTexture;
};
