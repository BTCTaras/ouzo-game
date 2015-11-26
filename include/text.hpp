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

	void SetColour(colour_t colour);
	void InitText(CFont *font, unsigned int size, const std::string &text);

	void SetText(const std::string &text);

	void Render(mvp_matrix_t &mvp);
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
