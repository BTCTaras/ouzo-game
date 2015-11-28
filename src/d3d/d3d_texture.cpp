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
	HRESULT result = D3DXCreateTextureFromFileEx(
		GFX_D3D_DEV,
		file,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&m_d3dtexture
	);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to load texture \"%s\"!!\n", file);
		m_d3dtexture = NULL;
		return;
	}

	D3DXIMAGE_INFO info;
	result = D3DXGetImageInfoFromFile(file, &info);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to retrieve image info from file \"%s\"!!\n", file);
		return;
	}

	m_width = info.Width;
	m_height = info.Height;
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