#pragma once

#include "scene.h"
#include "graphics.h"

#define GFX CGame::Inst->GetGraphics()

#include <memory>

struct GLFWwindow;

class CGame {
public:
	CGame();
	~CGame();

	static CGame *Inst;

	void StartLoop();
	void Stop();

	void SetTargetFPS(unsigned int fps);
	void SetVerticalSync(bool vsync);

	void SetScene(CScene *scene);
	S_CScene GetScene();

	CGraphics *GetGraphics();

	unsigned int GetWidth();
	unsigned int GetHeight();

	unsigned int m_width, m_height;

private:
	void InitGL();

	void OnRender();
	void OnUpdate();

	unsigned int m_targetFPS;
	float m_deltaTime;
	bool m_vsync;

	bool m_running;
	GLFWwindow *m_window;

	S_CScene m_scene;

	CGraphics m_graphics;
};
