#pragma once

#include "osu/osu_object.hpp"
#include "graphics_common/buffer.hpp"
#include "graphics_common/draw_attribs.hpp"
#include "graphics_common/texture.hpp"

#include <vector>

struct element_t {
	float x, y;
};

class COsuRenderer {
public:
	COsuRenderer();

	virtual void Init() = 0;
	virtual void DoRender(mvp_matrix_t &mvp) = 0;

	void AddElement(float x, float y);
	void RemoveElement(float x, float y);

protected:
	std::vector<element_t> m_elements;
	bool m_rebuildNeeded;

	virtual void RebuildInstances();
};

class CCircleRenderer : public COsuRenderer {
public:
	virtual void Init() override;
	virtual void DoRender(mvp_matrix_t &mvp) override;

	void SetCircleTexture(S_CTexture tex);

protected:
	virtual void RebuildInstances() override;

private:
	S_CBuffer m_buffer;
	S_CDrawAttribs m_drawAttribs;
	S_CTexture m_circleTex;
};