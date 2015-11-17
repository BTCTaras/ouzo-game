#include "game.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initialisation for stuff like GLFW and GLEW goes here.
// TODO: Un-hardcode this maybe?
bool initLibs() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW!!\n");
		return true;
	}

	if (!glewInit()) {
		fprintf(stderr, "Failed to initialise GLEW!!\n");
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {
	if (!initLibs()) {
		return 2;
	}

	CGame game;
	game.StartLoop();
	return 0;
}