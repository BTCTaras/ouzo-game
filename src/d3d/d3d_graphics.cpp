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
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0.0f);
	d3ddev->BeginScene();
}

void CD3DGraphics::Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program) {
	
}

void CD3DGraphics::End() {
}

void CD3DGraphics::EndScene() {
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

S_CProgram CD3DGraphics::GetDefaultProgram() {
	return nullptr;
}

S_CTexture CD3DGraphics::CreateTexture(const char * file) {
	return nullptr;
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

void CD3DGraphics::Draw(PrimitiveType primitive, S_CBuffer elementBuffer) {
}

void CD3DGraphics::SetTexture(S_CTexture tex, unsigned int slot) {
}

unsigned int CD3DGraphics::GetMaxTextureSize() {
	return 4096;
}

LPDIRECT3DDEVICE9 CD3DGraphics::GetDevice() {
	return d3ddev;
}

#endif
