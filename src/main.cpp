#include "game.hpp"
#include "scene_menu.hpp"

#include <stdio.h>

#include <GL/glew.h>

#include <SDL2/SDL_main.h>

int main(int argc, char *argv[]) {
	CGame game;
	CGame::Inst = &game;
	game.InitGame();
	game.SetScene(new CSceneMenu);
	game.StartLoop();
	return 0;
}
