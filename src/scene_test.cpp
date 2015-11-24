#include "scene_test.hpp"

#include <GL/glew.h>

void CSceneTest::OnInit() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CSceneTest::OnRender() {
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);
	glEnd();
}

void CSceneTest::OnResize(int newWidth, int newHeight) {
	printf("Resized to (%d, %d)\n", newWidth, newHeight);
}
