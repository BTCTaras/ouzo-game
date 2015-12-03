#include "game.hpp"
#include "scene_menu.hpp"
#include "scene_osu.hpp"

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

	CSceneOsu *osu = new CSceneOsu;
	osu->SetBackgroundTex(GFX->CreateTexture("assets/backgrounds/test.jpeg"));

	const float periods = 3.0f;

	for (float x = 0.0f; x < 1280.0f; x += 32.0f) {
		S_COsuObject circle = S_COsuObject(new COsuCircle(
			x - 32.0f,
			128.0f * sinf(((periods * 2) * M_PI / 1280.0f) * x) + 328.0f
		));

		osu->AddObject(circle);
	}

	game.SetScene(osu);

	game.StartLoop();
	return 0;
}
