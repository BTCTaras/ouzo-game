#pragma once

#include "graphics_common/texture.hpp"

#include <memory>

namespace GFX {
	const short BIND_WRITE = 0x0;
	const short BIND_READ = 0x1;
}

class CFrameBuffer {
public:
	virtual void Bind(short purpose) = 0;
	virtual void AssignToTexture(S_CTexture tex) = 0;

};

typedef std::shared_ptr<CFrameBuffer> S_CFrameBuffer;

class CGLFrameBuffer : public CFrameBuffer {
public:
	CGLFrameBuffer();
	~CGLFrameBuffer();

	virtual void Bind(short purpose) override;
	virtual void AssignToTexture(S_CTexture tex) override;

private:
	unsigned int m_id;
};

typedef std::shared_ptr<CGLFrameBuffer> S_CGLFrameBuffer;

