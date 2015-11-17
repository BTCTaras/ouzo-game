#pragma once

struct GLFWwindow;

class CGame {
public:
	CGame();
	~CGame();

	void StartLoop();
	void Stop();

	void SetTargetFPS(unsigned int fps);
	void SetVerticalSync(bool vsync);

private:
	void InitGL();

	void OnRender();
	void OnUpdate();

	unsigned int m_targetFPS;
	float m_deltaTime;
	bool m_vsync;

	bool m_running;
	GLFWwindow *m_window;
};