#ifdef _WIN32

#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"

#include "game.hpp"

#include <d3d9.h>
#include <Windows.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <vector>

CD3DGraphics::CD3DGraphics() {}

void CD3DGraphics::Init(SDL_Window *window) {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS));
	pp.hDeviceWindow = CGame::Inst->GetWindow_Win32();
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.EnableAutoDepthStencil = TRUE;
	pp.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		CGame::Inst->GetWindow_Win32(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&pp,
		&d3ddev
	);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create a Direct3D device!!\n");
		return;
	}

	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

CD3DGraphics::~CD3DGraphics() {
	d3ddev->Release();
	d3d->Release();
}

void CD3DGraphics::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	D3DVIEWPORT9 viewport;
	viewport.X = x;
	viewport.Y = y;
	viewport.Width = w;
	viewport.Height = h;
	d3ddev->SetViewport(&viewport);
}

void CD3DGraphics::BeginScene() {
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0x0, 1.0f, 0);
	d3ddev->BeginScene();
}

void CD3DGraphics::Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program) {
	m_currentBuffer = vertexBuffer;
}

void CD3DGraphics::End() {
	m_currentBuffer = nullptr;
}

void CD3DGraphics::EndScene() {
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

S_CProgram CD3DGraphics::GetDefaultProgram() {
	return nullptr;
}

S_CTexture CD3DGraphics::CreateTexture(const char *file) {
	S_CD3DTexture tex = S_CD3DTexture(new CD3DTexture);
	tex->LoadFromFile(file);
	return tex;
}

S_CShader CD3DGraphics::CreateShader(ShaderType type, const char *file) {
/*	S_CD3DShader shader = S_CD3DShader(new CD3DShader(type));
	shader->LoadFromFile(file);*/
	return nullptr;
}

S_CProgram CD3DGraphics::CreateProgram(size_t count, S_CShader *shaders) {
	/*S_CProgram program = S_CProgram(new CD3DProgram);
	program->LoadFromShaders(count, shaders);*/
	return nullptr;
}

S_CAtlasFactory CD3DGraphics::CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) {
	return nullptr;
}

S_CBuffer CD3DGraphics::CreateBuffer(BufferType type, BufferStorageType storageType) {
	return S_CD3DBuffer(new CD3DBuffer(type, storageType));
}

S_CMatrix CD3DGraphics::CreateIdentityMatrix() {
	return S_CMatrix();
}

void CD3DGraphics::Draw(PrimitiveType primitive, S_CBuffer elementBuffer) {
	D3DPRIMITIVETYPE prim = GetD3DPrimitiveType(primitive);
	S_CD3DBuffer buffer = std::static_pointer_cast<CD3DBuffer>(m_currentBuffer);

	d3ddev->SetFVF(CD3DBuffer::DEFAULT_BUFFER_FVF);
	d3ddev->SetStreamSource(0, buffer->m_buffer, 0, sizeof(vertex_t));
	d3ddev->DrawPrimitive(prim, 0, (UINT)GetPrimitiveCount(primitive, buffer->GetSize() / sizeof(vertex_t)));
}

void CD3DGraphics::SetTexture(S_CTexture tex, unsigned int slot) {
	S_CD3DTexture d3dtex = std::static_pointer_cast<CD3DTexture>(tex);
	d3ddev->SetTexture(slot, d3dtex->GetD3DHandle());
}

unsigned int CD3DGraphics::GetMaxTextureSize() {
	return 4096;
}

LPDIRECT3DDEVICE9 CD3DGraphics::GetDevice() {
	return d3ddev;
}

D3DPRIMITIVETYPE CD3DGraphics::GetD3DPrimitiveType(PrimitiveType type) {
	switch (type) {
	case PrimitiveType::GFX_LINES:
		return D3DPT_LINELIST;
	case PrimitiveType::GFX_LINE_STRIP:
		return D3DPT_LINESTRIP;
	case PrimitiveType::GFX_POINTS:
		return D3DPT_POINTLIST;
	case PrimitiveType::GFX_TRIANGLES:
		return D3DPT_TRIANGLELIST;
	case PrimitiveType::GFX_TRIANGLE_FAN:
		return D3DPT_TRIANGLEFAN;
	case PrimitiveType::GFX_TRIANGLE_STRIP:
		return D3DPT_TRIANGLESTRIP;
	default:
		// We have to do adventurous things like this because D3D is stupid.
		fprintf(stderr, "Unsupported primitive type %d!! Falling back to GFX_TRIANGLES.\n", type);
		return D3DPT_TRIANGLELIST;
	}
}

float CD3DGraphics::GetPrimitiveCount(PrimitiveType type, size_t vertexCount) {
	// We have to do adventurous things like this because D3D is stupid.
	switch (type) {
	case PrimitiveType::GFX_LINES:
		return vertexCount / 2.0f;

	case PrimitiveType::GFX_LINE_STRIP:
	case PrimitiveType::GFX_POINTS:
		return (float)vertexCount;

	case PrimitiveType::GFX_TRIANGLES:
		return vertexCount / 3.0f;

	case PrimitiveType::GFX_TRIANGLE_STRIP:
	case PrimitiveType::GFX_TRIANGLE_FAN:
		return vertexCount - 2.0f;
	}

	return 0.0f;
}

#endif
