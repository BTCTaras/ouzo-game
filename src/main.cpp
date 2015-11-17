#include "game.h"

#include <stdio.h>

#include <GLFW/glfw3.h>

// Initialisation for stuff like GLFW and GLEW goes here.
// TODO: Un-hardcode this maybe?
void initLibs() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW!!\n");
	}
}

int main(int argc, char *argv[]) {
	initLibs();

	CGame game;
	game.StartLoop();
	return 0;
}