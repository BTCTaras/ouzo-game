#include "uic_button.hpp"

CUIControlButton::CUIControlButton(CFont *font, S_CTexture texture, const std::string &text, unsigned int fontSize, colour_t colour, colour_t textColour) {
	this->m_text.reset(new CUIControlText(font, text, fontSize));
	//this->m_text->SetColour(colour);

	glyph_data_t *glyphData = font->GetGlyphData(text);

	this->m_background.reset(new CUIControlTexture(texture));

	this->AddChild(this->m_background);
	this->AddChild(this->m_text);
}

void CUIControlButton::SetText(const std::string &text) {
	this->m_text->SetText(text);
}

void CUIControlButton::SetColour(colour_t &colour) {
	//this->m_background->SetColour(colour);
}

void CUIControlButton::SetTextColour(colour_t &colour) {
	this->m_text->SetColour(colour);
}