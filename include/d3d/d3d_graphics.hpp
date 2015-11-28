#pragma once

#include "graphics.hpp"
#include "buffer.hpp"

#include <d3d11.h>
#include <dxgi.h>

#include <memory>

// Must be in order so D3D versions can be checked with lt and gt.
enum D3DVersion {
	WARP,
	D3D9,
	D3D10,
};

///
///	CD3DGraphics
///

class CD3DGraphics : public CGraphics {
public:
	CD3DGraphics(D3DVersion version);
	~CD3DGraphics();

	virtual void Init(SDL_Window *window) override;
	virtual void BeginScene() override;
	virtual void Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program = nullptr) override;
	virtual void End() override;
	virtual void EndScene() override;

	virtual S_CProgram GetDefaultProgram() override;
	virtual S_CTexture CreateTexture(const char *file = NULL) override;
	virtual S_CShader CreateShader(ShaderType type, const char *file = NULL) override;
	virtual S_CProgram CreateProgram(size_t count, S_CShader *shaders) override;
	virtual S_CAtlasFactory CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) override;
	virtual S_CBuffer CreateBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC) override;
	virtual void Draw(PrimitiveType primitive, S_CBuffer elementBuffer = nullptr) override;
	virtual void SetTexture(S_CTexture tex = nullptr, unsigned int slot = 0) override;
	virtual void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) override;

	virtual unsigned int GetMaxTextureSize() override;

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

	D3DVersion GetD3DVersion();

private:
	IDXGISwapChain *swapChain;
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;

	D3DVersion m_d3dversion;

	ID3D11RenderTargetView *m_backBuffer;
};

#define GFX_D3D std::static_pointer_cast<CD3DGraphics>(CGame::Inst->GetGraphics())
#define GFX_D3D_DEV GFX_D3D->GetDevice()

typedef std::shared_ptr<CD3DGraphics> S_CD3DGraphics;

///
///	CD3DBuffer
///

class CD3DBuffer : public CBuffer {
public:
	CD3DBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC);

	virtual void Orphan(size_t dataSize, void *data) override;
	virtual size_t GetSize() override;

private:
	size_t m_size;
};

typedef std::shared_ptr<CD3DBuffer> S_CD3DBuffer;

///
/// CD3DShader
///

class CD3DShader : public CShader {
public:
	CD3DShader(ShaderType type);
	~CD3DShader();

	virtual void LoadFromFile(const char *file) override;
	virtual void Load(const char *chunkName, const char *source) override;
	virtual const std::string& GetChunkName() override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

private:
	ID3D11DeviceChild *m_shader;
	std::string m_chunkName;
};

typedef std::shared_ptr<CD3DShader> S_CD3DShader;

///
///	CD3DProgram
///

class CD3DProgram : public CProgram {
public:
	virtual void LoadFromShaders(size_t count, S_CShader *shaders) override;
	virtual void Use() override;
	virtual void SetUniform(ShaderUniformType type, const char *name, void *values) override;
};

typedef std::shared_ptr<CD3DProgram> S_CD3DProgram;