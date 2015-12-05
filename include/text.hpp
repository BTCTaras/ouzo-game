#pragma once

#include "graphics.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "font.hpp"
#include "buffer.hpp"

#include <string>
#include <memory>

class CText {
public:
	CText();
	CText(CFont *font, unsigned int size, const std::string &text);
	~CText();

    ///
    /// Sets the colour of this text.
    ///
	void SetColour(colour_t colour);

	///
	/// Inititialises this CText with a font, size and text.
	///
	/// \param[in]  font    The font to use for this CText.
	/// \param[in]  size    The size of this CText in px.
	/// \param[in]  text    The text to be used for this CText.
	///
	void InitText(CFont *font, unsigned int size, const std::string &text);

    ///
    /// Changes the text this CText displays.
    ///
	void SetText(const std::string &text);

    ///
    /// Renders this text using a simple GFX->Draw call.
    ///
	void Render(mvp_matrix_t &mvp);

	///
	/// Returns the buffer containing this text's vertex data.
	///
	S_CBuffer GetBuffer();

private:
	void CreateTextBuffer();

	static S_CProgram s_fontProgram;

	std::string m_text;
	unsigned int m_fontBufferSize;
	unsigned int m_size;

	colour_t m_colour;

	CFont *m_font;

	S_CBuffer m_fontBuffer;
};

typedef std::shared_ptr<CFont> S_CFont;
