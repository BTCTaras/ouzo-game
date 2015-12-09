#pragma once

#include "osu/osu_object.hpp"
#include "osu/osu_render.hpp"

#include <vector>

typedef unsigned int osu_object_id_t;

class COsuBeatmap {
public:
	COsuBeatmap();

	osu_object_id_t AddObject(S_COsuObject obj);
	S_COsuObject GetOsuObject(osu_object_id_t id);

	void OnRender(mvp_matrix_t &mvp);

private:
	CCircleRenderer m_circleRenderer;
	std::vector<S_COsuObject> m_objects;
};