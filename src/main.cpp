#include "game.hpp"
#include "scene/scene_menu.hpp"
#include "scene/scene_osu.hpp"
#include "scene/scene_ctb.hpp"

#include <stdio.h>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mixer.h>

bool initLibs() {
	Uint32 sdlModules =
		SDL_INIT_VIDEO |
		SDL_INIT_AUDIO |
		SDL_INIT_GAMECONTROLLER |
		SDL_INIT_JOYSTICK;

	if (SDL_Init(sdlModules) != 0) {
		fprintf(stderr, "Failed to initialise SDL: %s\n", SDL_GetError());
		return false;
	}

	Uint32 mixFormats =
		MIX_INIT_OGG |
		MIX_INIT_MOD |
		MIX_INIT_FLAC |
		MIX_INIT_MP3;

	if (Mix_Init(mixFormats) < 0) {
		fprintf(stderr, "Failed to initialise SDL_mixer: %s\n", Mix_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		fprintf(stderr, "Failed to open SDL_mixer: %s\n", Mix_GetError());
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {
	if (!initLibs()) {
		fprintf(stderr, "One or more libraries couldn't be initialised!!\n");
#ifdef OUZO_DEBUG
		getchar();
#endif
		return 1;
	}

	CGame game;
	CGame::Inst = &game;

	game.InitGame(GraphicsAPI::OPENGL_CORE);

	CSceneOsu *osu = new CSceneOsu;
	osu->SetBackgroundTex(GFX->CreateTexture("assets/backgrounds/test.jpeg"));
	game.SetScene(osu);

	game.StartLoop();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
	return 0;
}
