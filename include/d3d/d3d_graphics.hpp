#pragma once

#include "graphics.hpp"
#include "buffer.hpp"

#include <d3d9.h>

#include <memory>

///
///	CD3DGraphics
///

class CD3DGraphics : public CGraphics {
public:
	CD3DGraphics();
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

	LPDIRECT3DDEVICE9 GetDevice();

private:
	static D3DPRIMITIVETYPE GetD3DPrimitiveType(PrimitiveType type);
	static float GetPrimitiveCount(PrimitiveType type, size_t vertexCount);

	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	S_CBuffer m_currentBuffer;
};

#define GFX_D3D std::static_pointer_cast<CD3DGraphics>(CGame::Inst->GetGraphics())
#define GFX_D3D_DEV GFX_D3D->GetDevice()

typedef std::shared_ptr<CD3DGraphics> S_CD3DGraphics;

///
///	CD3DBuffer
///

class CD3DBuffer : public CBuffer {
	friend class CD3DGraphics;
public:
	CD3DBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC);

	virtual void Orphan(size_t dataSize, void *data) override;
	virtual size_t GetSize() override;

	static const DWORD DEFAULT_BUFFER_FVF;

private:
	size_t m_size;
	LPDIRECT3DVERTEXBUFFER9 m_buffer;
};

typedef std::shared_ptr<CD3DBuffer> S_CD3DBuffer;

///
/// CTexture
///

class CD3DTexture : public CTexture {
public:
	CD3DTexture();
	~CD3DTexture();

	virtual void LoadFromFile(const char *file) override;

	virtual void Use() override;
	
	virtual unsigned int GetWidth() override;
	virtual unsigned int GetHeight() override;

	LPDIRECT3DTEXTURE9 GetD3DHandle();

private:
	LPDIRECT3DTEXTURE9 m_d3dtexture;
	unsigned int m_width, m_height;
};

typedef std::shared_ptr<CD3DTexture> S_CD3DTexture;

///
/// CD3DShader
///
/*
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
	static void LoadToBuffer(const char *file, char *buffer);
	static LARGE_INTEGER GetFileSize(const char *file);

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

typedef std::shared_ptr<CD3DProgram> S_CD3DProgram;*/