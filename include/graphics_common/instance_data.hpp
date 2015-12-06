#pragma once

#include "graphics_common/matrix.hpp"
#include "graphics_common/draw_attribs.hpp"

#include <vector>
#include <memory>

typedef unsigned int instance_id_t;

class CInstanceData {
public:
	CInstanceData();

	instance_id_t AddInstance(S_CMatrix matrix);
	S_CMatrix GetInstance(instance_id_t id, bool willModify = false);

	void LoadInto(S_CDrawAttribs attribs);

private:
	void CreateBuffers();

	std::vector<S_CMatrix> m_instances;
	S_CBuffer m_buffers[4];

	bool m_hasChanged;
};

typedef std::shared_ptr<CInstanceData> S_CInstanceData;