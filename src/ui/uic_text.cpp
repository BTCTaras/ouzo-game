#include "ui/uic_text.hpp"

CUIControlText::CUIControlText(CFont *font, const std::string &text, unsigned int size)
	: m_text(font, size, text),
	m_uiText(new CUIText(&m_text))
{
	this->AddRenderable(m_uiText);
}

void CUIControlText::SetText(const std::string &text) {
	m_uiText->SetText(text);
}