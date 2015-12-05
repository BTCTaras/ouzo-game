#pragma once

#include "ui.hpp"
#include "uic_text.hpp"
#include "uic_texture.hpp"

class CUIControlButton : public CUIControl {
public:
	CUIControlButton(
		CFont *font,
		S_CTexture texture,
		const std::string &text,
		unsigned int fontSize,
		colour_t colour = { 0.1f, 0.1f, 0.1f },
		colour_t textColour = { 1.0f, 1.0f, 1.0f }
	);
	CUIControlButton(
		CFont *font,
		S_CTexture texture,
		const std::string &text,
		unsigned int fontSize,
		unsigned int width,
		unsigned int height,
		colour_t colour = { 0.1f, 0.1f, 0.1f },
		colour_t textColour = { 1.0f, 1.0f, 1.0f }
	);

	void SetText(const std::string &text);
	void SetColour(colour_t &colour);
	void SetTextColour(colour_t &colour);
private:
	S_CUIControlText m_text;

	S_CUIControlTexture m_background;
};

typedef std::shared_ptr<CUIControlButton> S_CUIControlButton;