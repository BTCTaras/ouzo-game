#include "font.h"

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library s_freetype;

static void initFreeType() {
  bool done = false;
  if (!done) {
    int error = FT_Init_FreeType(&s_freetype);
    if (error) {
      fprintf(stderr, "Failed to initialise FreeType!!\n");
      goto done;
    }

    goto done;

    done:
      done = true;
      return;
  }
}

CFont::CFont(const char *file, unsigned int index) {
  initFreeType(); // Only initialises when needed. Okay to call every time.
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

  size_t strideBytes = size * size * (sizeof(unsigned char) * 3 + sizeof(unsigned int));
  size_t stride = size * size;

  unsigned char *texBuffer = (unsigned char*)malloc(strideBytes * charCount);
  printf("Allocating %d bytes of memory\n", strideBytes * charCount);

  if (texBuffer == NULL) {
    fprintf(stderr, "Could not allocate a font texture buffer! Are we out of memory?\n");
    return;
  }

  // Create the bitmaps
  FT_Set_Pixel_Sizes(m_fontFace, size, size);

  for (size_t i = 0; i < charCount; ++i) {
    unsigned long c = chars[i];
    unsigned int index = FT_Get_Char_Index(m_fontFace, c);

    int error = FT_Load_Glyph(m_fontFace, index, FT_LOAD_DEFAULT);
    if (error) {
      fprintf(stderr, "Error loading glyph for character %#06x!!\n", (unsigned int)c);
      continue;
    }

    error = FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_NORMAL);
    if (error) {
      fprintf(stderr, "Error rendering glyph for character %#06x!!\n", (unsigned int)c);
      continue;
    }

    FT_GlyphSlot glyph = m_fontFace->glyph;

    for (unsigned int i = 0; i < size * size; ++i) {
      unsigned char pixel = glyph->bitmap.buffer[i];
      texBuffer[i * stride + (i++)] = pixel;
    }
  }

  // Upload all glyphs to video memory
  glBindTexture(GL_TEXTURE_3D, handle);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, size, size, charCount, 0, GL_RGB, GL_UNSIGNED_BYTE, texBuffer);

  free(texBuffer);
}

CFont::~CFont() {

}
