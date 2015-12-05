#pragma once

#include "buffer.hpp"

#include <memory>

enum AttribType {
	POSITION,
	TEX_COORDS,
	NORMAL,
	INSTANCE
};

class CDrawAttribs {
public:
	virtual void SetSource(AttribType type, S_CBuffer buffer, size_t offset = 0) = 0;
};

typedef std::shared_ptr<CDrawAttribs> S_CDrawAttribs;

class CGLDrawAttribs : public CDrawAttribs {
public:
	CGLDrawAttribs();
	~CGLDrawAttribs();

	virtual void SetSource(AttribType type, S_CBuffer, size_t offset = 0) override;

	unsigned int GetOpenGLHandle();

private:
	unsigned int m_id;
};

typedef std::shared_ptr<CGLDrawAttribs> S_CGLDrawAttribs;
