#pragma once

#include "ui.hpp"
#include "uic_text.hpp"

class CUIControlButton : public CUIControl {
public:
	CUIControlButton(
		CFont *font,
		const std::string &text,
		unsigned int fontSize,
		colour_t colour = { 0.1f, 0.1f, 0.1f },
		colour_t textColour = { 1.0f, 1.0f, 1.0f }
	);
	CUIControlButton(
		CFont *font,
		const std::string &text,
		unsigned int fontSize,
		unsigned int width,
		unsigned int height,
		colour_t colour = { 0.1f, 0.1f, 0.1f },
		colour_t textColour = { 1.0f, 1.0f, 1.0f }
	);

	void SetText(const std::string &text);
	void SetColour(colour_t colour);

private:
	S_CUIControlText m_controlText;
};