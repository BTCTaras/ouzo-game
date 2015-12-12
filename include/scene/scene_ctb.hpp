#pragma once

#include "scene.hpp"
#include "scene/scene_gamemode.hpp"

#include "graphics_common/graphics.hpp"
#include "graphics_common/texture.hpp"

#include <vector>

struct fruit_t {
	float x, y;
};

class CSceneCTB : public CSceneGamemode {
public:
	CSceneCTB();

	virtual void OnInit() override;
	virtual void OnRender() override;
	virtual void OnUpdate() override;

	virtual void OnResize(int width, int height) override;

	virtual void OnClick(unsigned int button, float x, float y) override;

	virtual unsigned short GetGamemodeID() override;

private:
	std::vector<fruit_t> m_fruit;
	mvp_matrix_t m_mvpMatrix;
	S_CTexture m_fruitTex, m_bowlTex;
	S_CDrawAttribs m_quadAttribs;

	float m_dudeX, m_dudeY;
};