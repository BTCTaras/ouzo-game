#pragma once

#include "scene.hpp"
#include "graphics.hpp"
#include "texture.hpp"

#include <vector>

struct fruit_t {
	float x, y;
};

class CSceneCTB : public CScene {
public:
	CSceneCTB();

	virtual void OnInit() override;
	virtual void OnRender() override;
	virtual void OnUpdate() override;

	virtual void OnResize(int width, int height) override;

	virtual void OnClick(unsigned int button, float x, float y) override;

private:
	std::vector<fruit_t> m_fruit;
	mvp_matrix_t m_mvpMatrix;
	S_CTexture m_fruitTex, m_bowlTex;
	S_CDrawAttribs m_quadAttribs;

	float m_dudeX, m_dudeY;
};