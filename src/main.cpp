#include "game.h"
#include "scene_test.h"

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
	game.SetScene(new CSceneTest);
	game.StartLoop();
	return 0;
}