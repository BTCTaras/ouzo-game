#include "game.hpp"
#include "graphics_common/graphics.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <FreeImage.h>

const unsigned int GAME_START_WIDTH = 1280;
const unsigned int GAME_START_HEIGHT = 720;
const char *GAME_START_TITLE = "Ouzo";

CGame *CGame::Inst = nullptr;

CGame::CGame()
	:	m_targetFPS(60),
		m_running(true),
		m_sound(8)
{
}

void CGame::InitGame(GraphicsAPI api) {
	Uint32 windowFlags =
		((api == GraphicsAPI::OPENGL_CORE) ? SDL_WINDOW_OPENGL : (Uint32)NULL) |
		SDL_WINDOW_HIDDEN;

	m_window = SDL_CreateWindow(
		GAME_START_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		GAME_START_WIDTH,
		GAME_START_HEIGHT,
		windowFlags
	);

	if (!m_window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Couldn't create SDL window!!", NULL);
		return;
	}

    SDL_Surface *icon = SDL_LoadBMP("assets/icon.bmp");
    SDL_SetColorKey(icon, SDL_TRUE, SDL_MapRGB(icon->format, 255, 0, 255));
	SDL_SetWindowIcon(m_window, icon);
	SDL_FreeSurface(icon);

	switch (api) {
	case GraphicsAPI::OPENGL_CORE:
		m_graphics.reset(new CGLGraphics);
		break;
	}

	SDL_ShowWindow(m_window);

#ifdef _WIN32
	m_win32Window = GetActiveWindow();
#endif

	m_graphics->Init(m_window);

	this->SetVerticalSync(true); // On for now to prevent my gpu from screaming
}

#ifdef _WIN32
HWND CGame::GetWindow_Win32() {
	return m_win32Window;
}
#endif

CGame::~CGame() {
	SDL_DestroyWindow(m_window);
}

unsigned int CGame::GetWidth() {
	int width;
	SDL_GetWindowSize(m_window, &width, NULL);
	return width;
}

unsigned int CGame::GetHeight() {
	int height;
	SDL_GetWindowSize(m_window, NULL, &height);
	return height;
}

void CGame::Event_OnResize(int width, int height) {
	m_graphics->SetViewport(0, 0, width, height);

	if (this->GetScene() != nullptr) {
		this->GetScene()->OnResize(width, height);
	}
}

void CGame::Event_OnClick(int x, int y, int button, bool press) {
	this->GetScene()->OnClick(button, (float)x, (float)y);
}

void CGame::TranslateEvent(SDL_Event &event) {
	switch (event.type) {
	case SDL_QUIT:
		m_running = false;
		break;

	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
	{
		int button;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			button = 0;
			break;
		case SDL_BUTTON_RIGHT:
			button = 1;
			break;
		case SDL_BUTTON_MIDDLE:
			button = 2;
			break;
		default:
			button = 0xFF;
			break;
		}

		bool pressed = event.button.state == SDL_PRESSED;
		this->Event_OnClick(event.button.x, event.button.y, button, pressed);
		break;
	}

	case SDL_WINDOWEVENT:
		switch (event.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			this->Event_OnResize(event.window.data1, event.window.data2);
			break;
		}
		break;

	case SDL_APP_LOWMEMORY:
		abort(); // ALLAHU AKBAR
		break;
	}
}

double CGame::GetDeltaTime() {
	return m_deltaTime;
}

void CGame::StartLoop() {
	Uint32 lastTime = SDL_GetTicks();

	while (m_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			this->TranslateEvent(event);
		}

		Uint32 now = SDL_GetTicks();
		Uint32 dt = now - lastTime; // This is the time difference between last frame and this frame.
		m_deltaTime = dt / 1000.0;

		this->OnUpdate();
		this->OnRender(&lastTime);

		if (!m_vsync) {
			// Give the CPU a short break
			// TODO: Make this configurable? Some people like having a million FPS...
			SDL_Delay(1);
		}
	}
}

void CGame::Stop() {
	m_running = false;
}

void CGame::SetTargetFPS(unsigned int fps) {
	m_targetFPS = fps;
}

// TODO: Move this to CGraphics
void CGame::SetVerticalSync(bool vsync) {
	m_vsync = vsync;
	int interval;

	if (vsync) {
		if (glewIsSupported("WGL_EXT_swap_control_tear") || glewIsSupported("GLX_EXT_swap_control_tear")) {
			printf("EXT_swap_control_tear is available!! Using late swap tearing!\n");
			interval = -1;
		} else {
			interval = 1;
		}
	}
	else {
		interval = 0;
	}

	if (SDL_GL_SetSwapInterval(interval) == -1) {
		fprintf(stderr, "Failed to set swap interval: %s\n", SDL_GetError());
		fprintf(stderr, "Disabling VSync\n");
		m_vsync = false;
		return;
	}
}

S_CGraphics CGame::GetGraphics() {
	return m_graphics;
}

CSoundManager* CGame::GetSound() {
	return &m_sound;
}

void CGame::SetScene(CScene *scene) {
	if (m_scene != nullptr) {
		m_scene->OnLeave(scene);
	}

	m_scene.reset(scene);
	m_scene->OnInit();

	int width, height;
	SDL_GetWindowSize(m_window, &width, &height);
	m_scene->OnResize(width, height);
}

S_CScene CGame::GetScene() {
	return m_scene;
}

void CGame::OnRender(Uint32 *swapTime) {
	m_graphics->BeginScene();

	// This seems wrong but it works so if weird shit happens,
	// it's this thing's fault
	if (swapTime != NULL) {
		*swapTime = SDL_GetTicks();
	}

	m_scene->OnRender();
	m_graphics->EndScene();
}

void CGame::OnUpdate() {
	m_scene->OnUpdate();
}
