#include "game.hpp"
#include "scene_menu.hpp"

#include <stdio.h>

#include <GL/glew.h>

int main(int argc, char *argv[]) {
	CGame game;
	CGame::Inst = &game;
	game.InitGame();
	game.SetScene(new CSceneMenu);
	game.StartLoop();
	return 0;
}

#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	char *args[1] = { "no" };
	return main(0, args); // no arguments for windows because it's a cunt
}

#endif
