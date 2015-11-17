#include "game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

const unsigned int GAME_START_WIDTH = 1280;
const unsigned int GAME_START_HEIGHT = 720;
const char *GAME_START_TITLE = "Ouzo";

CGame::CGame()
	: m_running(true)
{
	m_window = glfwCreateWindow(GAME_START_WIDTH, GAME_START_HEIGHT, GAME_START_TITLE, NULL, NULL);

	if (!m_window) {
		fprintf(stderr, "ERROR: Could not create window!!\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(m_window);
}

CGame::~CGame() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void CGame::StartLoop() {
	while (!glfwWindowShouldClose(m_window) && m_running) {
		// Game loop code here

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void CGame::Stop() {
	m_running = false;
}
