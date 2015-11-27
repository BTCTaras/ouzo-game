#ifdef _WIN32

#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"

#include "game.hpp"

#include <d3d11.h>
#include <dxgi.h>
#include <Windows.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <vector>

CD3DGraphics::CD3DGraphics(D3DVersion version)
	: m_d3dversion(version)
{
}

void CD3DGraphics::Init(SDL_Window *window) {

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = CGame::Inst->GetWindow_Win32();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.SampleDesc.Count = 4;

	std::vector<D3D_FEATURE_LEVEL> levels;

	if (m_d3dversion >= D3DVersion::D3D9) {
		levels.push_back(D3D_FEATURE_LEVEL_9_1);
		levels.push_back(D3D_FEATURE_LEVEL_9_2);
		levels.push_back(D3D_FEATURE_LEVEL_9_3);
	}

	if (m_d3dversion >= D3DVersion::D3D10) {
		levels.push_back(D3D_FEATURE_LEVEL_10_0);
		levels.push_back(D3D_FEATURE_LEVEL_10_1);
	}

	D3D_FEATURE_LEVEL createdFL;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL,
		(m_d3dversion == D3DVersion::WARP) ?
		D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE,
		NULL,
#ifdef OUZO_DEBUG
		D3D11_CREATE_DEVICE_DEBUG,
#else
		0,
#endif
		&levels[0],
		levels.size(),
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&dev,
		&createdFL,
		&devcon
	);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create a Direct3D device!!\n");
		return;
	}

	ID3D11Texture2D *pBackBuffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	result = dev->CreateRenderTargetView(pBackBuffer, NULL, &m_backBuffer);
	pBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &m_backBuffer, NULL);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create the back buffer!!\n");
		return;
	}
}

CD3DGraphics::~CD3DGraphics() {
	swapChain->Release();
	m_backBuffer->Release();
	devcon->Release();
	dev->Release();
}

void CD3DGraphics::SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
	if (devcon != NULL) {
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = (FLOAT)x;
		viewport.TopLeftY = (FLOAT)y;
		viewport.Width = (FLOAT)w;
		viewport.Height = (FLOAT)h;

		devcon->RSSetViewports(1, &viewport);
	}
}

void CD3DGraphics::BeginScene() {
	FLOAT clearColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	devcon->ClearRenderTargetView(m_backBuffer, clearColour);
}

void CD3DGraphics::Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program) {
	
}

void CD3DGraphics::End() {
}

void CD3DGraphics::EndScene() {
	swapChain->Present(0, 0);
}

S_CProgram CD3DGraphics::GetDefaultProgram() {
	return nullptr;
}

S_CTexture CD3DGraphics::CreateTexture(const char * file) {
	return nullptr;
}

S_CShader CD3DGraphics::CreateShader(ShaderType type, const char * file) {
	return nullptr;
}

S_CProgram CD3DGraphics::CreateProgram(size_t count, S_CShader * shaders) {
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

ID3D11Device* CD3DGraphics::GetDevice() {
	return dev;
}

ID3D11DeviceContext* CD3DGraphics::GetDeviceContext() {
	return devcon;
}

D3DVersion CD3DGraphics::GetD3DVersion() {
	return m_d3dversion;
}

#endif