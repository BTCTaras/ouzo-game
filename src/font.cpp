#include "font.h"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library s_freetype = NULL;
static size_t s_fontRefCount = 0;

static void initFreeType() {
  if (s_freetype == NULL) {
    int error = FT_Init_FreeType(&s_freetype);
    if (error) {
      fprintf(stderr, "Failed to initialise FreeType!!\n");
    }
  }
}

CFont::CFont(const char *file, unsigned int index) {
  initFreeType(); // Only initialises when needed. Okay to call every time.
  s_fontRefCount++;
  this->LoadFromFile(file, index);
}

void CFont::LoadFromFile(const char *file, unsigned int index) {
  int error = FT_New_Face(s_freetype, file, index, &m_fontFace);

  switch (error) {
    case FT_Err_Unknown_File_Format:
      fprintf(stderr, "Font \"%s\" has an unknown format!!\n", file);
      return;

    case 0: // No error
      break;

    default: // Unknown, though probably can't find the file.
      fprintf(stderr, "Failed to load font \"%s\"!!\n", file);
      return;
  }
}

void CFont::CreateGlyphTexture(CTexture *tex, unsigned int size, size_t charCount, unsigned long *chars) {
  unsigned int handle = tex->GetOpenGLHandle();

  if (handle <= 0) {
    fprintf(stderr, "CFont::CreateGlyphTexture: Texture passed has invalid OpenGL handle!!\n");
    return;
  }

  // Create the bitmaps
  FT_Set_Pixel_Sizes(m_fontFace, size, size);

  // Upload all glyphs to video memory
  glBindTexture(GL_TEXTURE_2D, handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  const size_t texSize = size * size * charCount * sizeof(unsigned char);
  unsigned char texBuf[texSize];
  memset(texBuf, 0, texSize);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size * charCount, size, 0, GL_RED, GL_UNSIGNED_BYTE, texBuf);

  for (size_t i = 0; i < charCount; ++i) {
    unsigned long c = chars[i];

    int error = FT_Load_Char(m_fontFace, c, FT_LOAD_RENDER);
    if (error) {
      fprintf(stderr, "Error loading glyph for character %#010x!!\n", (unsigned int)c);
      continue;
    }

    FT_GlyphSlot g = m_fontFace->glyph;
    glTexSubImage2D(GL_TEXTURE_2D, 0, size * i, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
  }
}

CFont::~CFont() {
  s_fontRefCount--;
  FT_Done_Face(m_fontFace);

  if (s_freetype != NULL && s_fontRefCount == 0) {
    FT_Done_FreeType(s_freetype);
  }
}
