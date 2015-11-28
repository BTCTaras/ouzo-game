#ifdef _WIN32
#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"
#include "graphics.hpp"
#include "game.hpp"

#include <Windows.h>
#include <d3d9.h>

const DWORD CD3DBuffer::DEFAULT_BUFFER_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

CD3DBuffer::CD3DBuffer(BufferType type, BufferStorageType storageType)
	: CBuffer(type, storageType)
{
	HRESULT result;

	switch (type) {
	case BufferType::VERTEX_BUFFER:
		result = GFX_D3D_DEV->CreateVertexBuffer(
			3 * sizeof(vertex_t),
			D3DUSAGE_WRITEONLY,
			DEFAULT_BUFFER_FVF,
			D3DPOOL_DEFAULT,
			(IDirect3DVertexBuffer9**)&m_buffer,
			NULL
		);
		break;

	case BufferType::ELEMENT_BUFFER:
		result = GFX_D3D_DEV->CreateIndexBuffer(
			sizeof(unsigned short),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			(IDirect3DIndexBuffer9**)&m_buffer,
			NULL
		);
	}
	
	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create buffer!!\n");
		return;
	}
}

CD3DBuffer::~CD3DBuffer() {
	if (m_buffer != NULL) {
		m_buffer->Release();
	}
}

void CD3DBuffer::Orphan(size_t dataSize, void *data) {
	VOID *pData;

	// We have to do adventurous things like this because D3D is stupid.
	switch (m_type) {
	case BufferType::VERTEX_BUFFER:
	{
		LPDIRECT3DVERTEXBUFFER9 vertBuf = (LPDIRECT3DVERTEXBUFFER9)m_buffer;
		vertBuf->Lock(0, 0, &pData, D3DLOCK_DISCARD);
		break;
	}

	case BufferType::ELEMENT_BUFFER:
	{
		LPDIRECT3DINDEXBUFFER9 indexBuf = (LPDIRECT3DINDEXBUFFER9)m_buffer;
		indexBuf->Lock(0, 0, &pData, D3DLOCK_DISCARD);
		break;
	}
	}

	memcpy(pData, data, dataSize);

	switch (m_type) {
	case BufferType::VERTEX_BUFFER:
	{
		LPDIRECT3DVERTEXBUFFER9 vertBuf = (LPDIRECT3DVERTEXBUFFER9)m_buffer;
		vertBuf->Unlock();
		break;
	}

	case BufferType::ELEMENT_BUFFER:
	{
		LPDIRECT3DINDEXBUFFER9 indexBuf = (LPDIRECT3DINDEXBUFFER9)m_buffer;
		indexBuf->Unlock();
		break;
	}
	}
	
	m_size = dataSize;
}

size_t CD3DBuffer::GetSize() {
	return m_size;
}

#endif