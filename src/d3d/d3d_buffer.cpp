#ifdef _WIN32
#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"
#include "graphics.hpp"
#include "game.hpp"

#include <Windows.h>
#include <d3d9.h>

static const DWORD DEFAULT_BUFFER_FVF(D3DFVF_XYZ | D3DFVF_TEX0);

CD3DBuffer::CD3DBuffer(BufferType type, BufferStorageType storageType)
	: CBuffer(type, storageType)
{
	/*
	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create vertex buffer!!\n");
		return;
	}*/
}

void CD3DBuffer::Orphan(size_t dataSize, void *data) {
	m_size = dataSize;
}

size_t CD3DBuffer::GetSize() {
	return m_size;
}

#endif