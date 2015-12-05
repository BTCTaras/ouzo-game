#pragma once

#include "texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <memory>

struct glyph_data_t {
	unsigned int width, height;
	unsigned int bitmapLeft, bitmapTop;
	unsigned int advanceX, advanceY;
	float u1, v1, u2, v2;
};

class CFont {
public:
	CFont();

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
	/// Creates a texture atlas of characters for use in OpenGL.
	/// \param[out] tex       The final texture.
	/// \param[in]  size      The size of each glyph in pixels.
	/// \param[in]  charCount The amount of characters contained in chars.
	/// \param[in]  chars     The chars to be rendered as glyphs.
	///
	S_CTexture CreateAtlas(unsigned int size);

	///
	/// Retrieves a texture atlas from the atlas cache.
	/// You must call CreateAtlas before calling this.
	///
	/// \param[in]  size  Get an atlas with this size.
	///
	S_CTexture GetAtlas(unsigned int size);

	///
	/// Returns glyph information about a certain character.
	///
	glyph_data_t* GetGlyphData(unsigned long character);

private:
	FT_Face m_fontFace;

	std::map<unsigned long, glyph_data_t> m_glyphData;
	std::map<unsigned int, S_CTexture> m_atlasCache;
};

typedef std::shared_ptr<CFont> S_CFont;
