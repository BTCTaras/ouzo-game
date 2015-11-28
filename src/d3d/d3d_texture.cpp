#ifdef _WIN32

#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"
#include "game.hpp"
#include "graphics.hpp"

#include <d3d9.h>
#include <d3dx9.h>

CD3DTexture::CD3DTexture()
	: m_d3dtexture(NULL)
{
}

CD3DTexture::~CD3DTexture() {
	if (m_d3dtexture != NULL) {
		m_d3dtexture->Release();
	}
}

void CD3DTexture::LoadFromFile(const char *file) {
	HRESULT result = D3DXCreateTextureFromFile(GFX_D3D_DEV, file, &m_d3dtexture);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to load texture \"%s\"!!\n", file);
		m_d3dtexture = NULL;
		return;
	}
}

void CD3DTexture::Use() {
	GFX_D3D_DEV->SetTexture(0, m_d3dtexture);
}

unsigned int CD3DTexture::GetWidth() {
	return m_width;
}

unsigned int CD3DTexture::GetHeight() {
	return m_height;
}

LPDIRECT3DTEXTURE9 CD3DTexture::GetD3DHandle() {
	return m_d3dtexture;
}

#endif