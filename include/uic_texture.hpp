#pragma once

#include "ui.hpp"

class CUIControlTexture : public CUIControl {
public:
	CUIControlTexture(S_CTexture texture = nullptr);

	void SetTexture(S_CTexture texture);

private:
	S_CUISprite m_sprite;
};

typedef std::shared_ptr<CUIControlTexture> S_CUIControlTexture;