#include "game.hpp"
#include "scene_menu.hpp"

#include <stdio.h>

#include <GL/glew.h>
#undef main

int main(int argc, char *argv[]) {
	CGame game;
	CGame::Inst = &game;
	game.InitGame();
	game.SetScene(new CSceneMenu);
	game.StartLoop();
	return 0;
}
