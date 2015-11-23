#include "game.h"
#include "scene_menu.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initialisation for stuff like GLFW goes here.
// TODO: Un-hardcode this maybe?
bool initLibs() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW!!\n");
		return true;
	}

	return true;
}

int main(int argc, char *argv[]) {
	if (!initLibs()) {
#ifdef OUZO_DEBUG
		getchar(); // chance to read the error
#endif
		return 2;
	}

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
