#pragma once

#include "buffer.hpp"
#include "matrix.hpp"

#include <memory>
#include <map>

enum AttribType {
	// regular vertex attribs
	POSITION = 0,
	TEX_COORDS = 1,
	NORMAL = 2,

	// used for instancing
	OFFSET1 = 3,
	OFFSET2 = 4,
	OFFSET3 = 5,
	OFFSET4 = 6
};

struct offset_t {
	float x, y, z, w;
};

class CDrawAttribs {
public:
	virtual void SetSource(AttribType type, S_CBuffer buffer, size_t offset = 0) = 0;
	virtual S_CBuffer GetSource(AttribType type) = 0;
};

typedef std::shared_ptr<CDrawAttribs> S_CDrawAttribs;

class CGLDrawAttribs : public CDrawAttribs {
public:
	CGLDrawAttribs();
	~CGLDrawAttribs();

	virtual void SetSource(AttribType type, S_CBuffer buffer, size_t offset = 0) override;
	virtual S_CBuffer GetSource(AttribType type) override;

	unsigned int GetOpenGLHandle();

private:
	unsigned int m_id;

	// needed to prevent the buffers from being destroyed while they're in use
	std::map<AttribType, S_CBuffer> m_buffers;

};

typedef std::shared_ptr<CGLDrawAttribs> S_CGLDrawAttribs;
