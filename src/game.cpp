#include "game.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

const unsigned int GAME_START_WIDTH = 1280;
const unsigned int GAME_START_HEIGHT = 720;
const char *GAME_START_TITLE = "Ouzo";

CGame *CGame::Inst = nullptr;

static void GLFW_OnResize(GLFWwindow *window, int width, int height);

CGame::CGame()
	:	m_running(true),
		m_targetFPS(60)
{
	m_window = glfwCreateWindow(GAME_START_WIDTH, GAME_START_HEIGHT, GAME_START_TITLE, NULL, NULL);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	if (!m_window) {
		fprintf(stderr, "ERROR: Could not create window!!\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(m_window);

	static bool inittedGLEW = false;

	if (!inittedGLEW) {
		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialise GLEW!!\n");
			exit(2);
			return;
		}

		inittedGLEW = true;
	}

	const GLubyte *glVersion = glGetString(GL_VERSION);
	printf("Graphics Driver: %s\n", glVersion);

	glfwSetWindowSizeCallback(m_window, GLFW_OnResize);

	this->SetVerticalSync(false);
	this->InitGL();
}

CGame::~CGame() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void GLFW_OnResize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	CGame::Inst->GetScene()->OnResize(width, height);
}

void CGame::StartLoop() {
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window) && m_running) {
		double now = glfwGetTime();
		double dt = now - lastTime;
		m_deltaTime = (float)dt;

		this->OnUpdate();
		this->OnRender();

		lastTime = glfwGetTime();

		glfwSwapBuffers(m_window);
		glfwPollEvents();

		// Give the CPU a short break
		// TODO: Make this configurable? Some people like having a million FPS...
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void CGame::Stop() {
	m_running = false;
}

void CGame::SetTargetFPS(unsigned int fps) {
	m_targetFPS = fps;
}

void CGame::SetVerticalSync(bool vsync) {
	m_vsync = vsync;
	glfwSwapInterval(vsync ? 1 : 0);
}

void CGame::SetScene(CScene *scene) {
	if (m_scene != nullptr) {
		m_scene->OnLeave(scene);
	}

	m_scene.reset(scene);
	m_scene->OnInit();
}

S_CScene CGame::GetScene() {
	return m_scene;
}

void CGame::InitGL() {
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
}

void CGame::OnRender() {
	glClear(GL_COLOR_BUFFER_BIT);
	m_scene->OnRender();
}

void CGame::OnUpdate() {
	m_scene->OnUpdate();
}
