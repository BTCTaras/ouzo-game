#pragma once

#include <GLFW/glfw3.h>

class CGame {
public:
	CGame();
	~CGame();

	void StartLoop();
	void Stop();

private:
	bool m_running;
	GLFWwindow *m_window;
};