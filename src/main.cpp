#include "game.hpp"
#include "scene_menu.hpp"

#include <stdio.h>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#undef main

int main(int argc, char *argv[]) {
	Uint32 sdlModules =
		SDL_INIT_VIDEO |
		SDL_INIT_AUDIO |
		SDL_INIT_GAMECONTROLLER |
		SDL_INIT_JOYSTICK;
								
	if (SDL_Init(sdlModules) != 0) {
		fprintf(stderr, "Failed to initialise SDL: %s\n", SDL_GetError());
	}

	CGame game;
	CGame::Inst = &game;

#ifdef _WIN32
	game.InitGame(GraphicsAPI::DIRECT3D9);
#else
	game.InitGame(GraphicsAPI::OPENGL_CORE);
#endif

	game.SetScene(new CSceneMenu);
	game.StartLoop();
	return 0;
}
