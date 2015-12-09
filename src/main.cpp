#include "game.hpp"
#include "scene/scene_menu.hpp"
#include "scene/scene_osu.hpp"
#include "scene/scene_ctb.hpp"

#include <stdio.h>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

int main(int argc, char *argv[]) {
	Uint32 sdlModules =
		SDL_INIT_VIDEO |
		SDL_INIT_AUDIO |
		SDL_INIT_GAMECONTROLLER |
		SDL_INIT_JOYSTICK;
								
	if (SDL_Init(sdlModules) != 0) {
		fprintf(stderr, "Failed to initialise SDL: %s\n", SDL_GetError());
		return 1;
	}

	CGame game;
	CGame::Inst = &game;

	game.InitGame(GraphicsAPI::OPENGL_CORE);

	//CSceneOsu *osu = new CSceneOsu;
	//osu->SetBackgroundTex(GFX->CreateTexture("assets/backgrounds/test.jpeg"));
	game.SetScene(new CSceneMenu);

	game.StartLoop();
	return 0;
}
