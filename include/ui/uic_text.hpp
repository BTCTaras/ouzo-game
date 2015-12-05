#pragma once

#include "ui.hpp"

class CUIControlText : public CUIControl {
public:
	CUIControlText(CFont *font, const std::string &text, unsigned int size);

	void SetText(const std::string &text);

private:
	CText m_text;
	S_CUIText m_uiText;
};

typedef std::shared_ptr<CUIControlText> S_CUIControlText;