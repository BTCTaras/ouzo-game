#include "game.hpp"

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
static void GLFW_OnClick(GLFWwindow *window, int button, int action, int mods);

CGame::CGame()
	:	m_targetFPS(60),
		m_running(true)
{
}

void CGame::InitGame() {
	// We want to target OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// We don't want deprecated OpenGL functions.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Forbid compatibility

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);


	m_window = glfwCreateWindow(GAME_START_WIDTH,
															GAME_START_HEIGHT,
															GAME_START_TITLE,
															NULL,	// No fullscreen.
															NULL // Don't share the context with anyone.
														);

	// Centre the window
	const GLFWvidmode *vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_window,
		vidmode->width / 2 - GAME_START_WIDTH / 2,
		vidmode->height / 2 - GAME_START_HEIGHT / 2
	);

	if (!m_window) {
		fprintf(stderr, "ERROR: Could not create window!!\n");
		glfwTerminate();
		exit(1);
	}

	// Make sure our GL calls go to this window.
	glfwMakeContextCurrent(m_window);

	static bool inittedGLEW = false;

	// Initialise GLEW if needed.
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
	glfwSetMouseButtonCallback(m_window, GLFW_OnClick);

	this->SetVerticalSync(true); // On for now to prevent my gpu from screaming
	this->InitGL();

	m_graphics.reset(new CGLGraphics);
	m_graphics->Init();

	glfwShowWindow(m_window);
}

CGame::~CGame() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

unsigned int CGame::GetWidth() {
	return m_width;
}

unsigned int CGame::GetHeight() {
	return m_height;
}

void GLFW_OnResize(GLFWwindow *window, int width, int height) {
	CGame::Inst->m_width = width;
	CGame::Inst->m_height = height;
	glViewport(0, 0, width, height);

	if (CGame::Inst->GetScene() != nullptr) {
		CGame::Inst->GetScene()->OnResize(width, height);
	}
}

void GLFW_OnClick(GLFWwindow *window, int button, int action, int mods) {
	unsigned int translatedBtn;
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			translatedBtn = 0;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			translatedBtn = 1;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			translatedBtn = 2;
			break;
		default:
			translatedBtn = 0xFF;
			break;
	}

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	CGame::Inst->GetScene()->OnClick(translatedBtn, (float)x, (float)y);
}

void CGame::StartLoop() {
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_window) && m_running) {
		double now = glfwGetTime();
		double dt = now - lastTime; // This is the time difference between last frame and this frame.
		m_deltaTime = (float)dt;

		this->OnUpdate();
		this->OnRender();

		lastTime = glfwGetTime();

		m_graphics->EndScene();
		glfwSwapBuffers(m_window); // TODO: Replace this with CGraphics::EndScene
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

S_CGraphics CGame::GetGraphics() {
	return m_graphics;
}

void CGame::SetScene(CScene *scene) {
	if (m_scene != nullptr) {
		m_scene->OnLeave(scene);
	}

	m_scene.reset(scene);
	m_scene->OnInit();

	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	m_scene->OnResize(width, height);
}

S_CScene CGame::GetScene() {
	return m_scene;
}

void CGame::InitGL() {
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
}

void CGame::OnRender() {
	m_graphics->BeginScene();
	m_scene->OnRender();
}

void CGame::OnUpdate() {
	m_scene->OnUpdate();
}
