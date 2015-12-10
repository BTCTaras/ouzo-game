#include "osu/osu_beatmap.hpp"

#include "graphics_common/graphics.hpp"
#include "game.hpp"

COsuBeatmap::COsuBeatmap() {
	m_circleRenderer.Init();
	m_circleRenderer.SetCircleTexture(GFX->CreateTexture("assets/gamemodes/standard/circle.png"));
	m_circleRenderer.AddElement(64.0f, 64.0f);
}

osu_object_id_t COsuBeatmap::AddObject(S_COsuObject obj) {
	osu_object_id_t id = (osu_object_id_t)m_objects.size();
	m_objects.push_back(obj);
	return id;
}

S_COsuObject COsuBeatmap::GetOsuObject(osu_object_id_t id) {
	if (id < m_objects.size()) {
		return m_objects[id];
	}

	return nullptr;
}

void COsuBeatmap::OnRender(mvp_matrix_t &mvp) {
	mvp.model->LoadIdentity();
	m_circleRenderer.DoRender(mvp);
}
