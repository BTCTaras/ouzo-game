#include "font.hpp"
#include "font_chars.hpp"
#include "atlas_factory.hpp"
#include "graphics.hpp"
#include "game.hpp"

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

CFont::CFont() {
  initFreeType(); // Only initialises when needed. Okay to call every time.
  s_fontRefCount++;
}

CFont::CFont(const char *file, unsigned int index)
  : CFont()
{
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

S_CTexture CFont::CreateAtlas(unsigned int size) {
  // Create the bitmaps
  FT_Set_Pixel_Sizes(m_fontFace, size, size);

  size_t charCount = sizeof(FONT_CHARS) / sizeof(FONT_CHARS[0]);
  const float atlasWidth = (float)(size * charCount);
  const float atlasHeight = (float)size;

  const unsigned int maxTextureSize = GFX->GetMaxTextureSize();

  if (atlasWidth * atlasHeight > maxTextureSize * maxTextureSize) {
    fprintf(stderr, "Font atlas is too big to upload to the GPU!!\n");
    return nullptr;
  }

  S_CAtlasFactory atlas = GFX->CreateAtlasFactory((unsigned int)atlasWidth, (unsigned int)atlasHeight, 1);

  for (size_t i = 0; i < charCount; ++i) {
    unsigned long c = FONT_CHARS[i];

    int error = FT_Load_Char(m_fontFace, c, FT_LOAD_RENDER);
    if (error) {
      fprintf(stderr, "Error loading glyph for character %#010x!!\n", (unsigned int)c);
      continue;
    }

    FT_GlyphSlot g = m_fontFace->glyph;

    glyph_data_t gdata;
    gdata.width = g->bitmap.width;
    gdata.height = g->bitmap.rows;
    gdata.bitmapLeft = g->bitmap_left;
    gdata.bitmapTop = g->bitmap_top;
    gdata.advanceX = g->advance.x;
    gdata.advanceY = g->advance.y;
	gdata.u1 = (size * i) / atlasWidth;
	gdata.v1 = 0.0f;
	gdata.u2 = (size * i + gdata.width) / atlasWidth;
	gdata.v2 = gdata.height / atlasHeight;
    m_glyphData[c] = gdata;

    atlas->Write(size * i, 0, gdata.width, gdata.height, g->bitmap.buffer);
  }

  S_CTexture tex = atlas->GetTexture(FilterType::LINEAR);
  m_atlasCache[size] = tex;
  return tex;
}

S_CTexture CFont::GetAtlas(unsigned int size) {
  if (m_atlasCache.find(size) == m_atlasCache.end()) {
    fprintf(stderr, "CFont::GetAtlas: No atlas with size %d!!\n", size);
    return nullptr;
  }

  return m_atlasCache[size];
}

glyph_data_t* CFont::GetGlyphData(unsigned long character) {
  return &m_glyphData[character];
}

CFont::~CFont() {
  s_fontRefCount--;
  FT_Done_Face(m_fontFace);

  if (s_freetype != NULL && s_fontRefCount == 0) {
    FT_Done_FreeType(s_freetype);
  }
}
