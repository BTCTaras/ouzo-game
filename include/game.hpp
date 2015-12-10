#pragma once

#include "scene.hpp"
#include "graphics_common/graphics.hpp"
#include "audio/sound_manager.hpp"

#define GFX CGame::Inst->GetGraphics()
#define SFX CGame::Inst->GetSound()

#ifdef _WIN32
#include <Windows.h>
#endif

#include <memory>
#include <SDL2/SDL.h>

struct GLFWwindow;

enum GraphicsAPI {
	OPENGL_CORE,
	
#ifdef _WIN32
	DIRECT3D9
#endif
};

class CGame {
public:
	CGame();
	~CGame();

	static CGame *Inst;

	void InitGame(GraphicsAPI api);

	void StartLoop();
	void Stop();

	void SetTargetFPS(unsigned int fps);
	void SetVerticalSync(bool vsync);

	void SetScene(CScene *scene);
	S_CScene GetScene();

	float GetDeltaTime();

	S_CGraphics GetGraphics();
	CSoundManager* GetSound();

	unsigned int GetWidth();
	unsigned int GetHeight();

	unsigned int m_width, m_height;

#ifdef _WIN32
	HWND GetWindow_Win32();
#endif

private:
	void Event_OnResize(int width, int height);
	void Event_OnClick(int x, int y, int button, bool press);

	void TranslateEvent(SDL_Event &event);

	SDL_Surface *LoadIconFromFile(const char *file);

	void OnRender(Uint32 *swapTime);
	void OnUpdate();

	unsigned int m_targetFPS;
	float m_deltaTime;
	bool m_vsync;

	bool m_running;
	SDL_Window *m_window;

	S_CScene m_scene;
	S_CGraphics m_graphics;
	CSoundManager m_sound;

#ifdef _WIN32
	HWND m_win32Window;
#endif
};
