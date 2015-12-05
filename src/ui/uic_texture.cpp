#include "ui/uic_texture.hpp"

CUIControlTexture::CUIControlTexture(S_CTexture tex)
	: m_sprite(new CUISprite(nullptr))
{
	if (tex != nullptr) {
		m_sprite->SetTexture(tex);
		this->width = (float)tex->GetWidth();
		this->height = (float)tex->GetHeight();
	}

	this->AddRenderable(m_sprite);
}

void CUIControlTexture::SetTexture(S_CTexture tex) {
	m_sprite->SetTexture(tex);
}