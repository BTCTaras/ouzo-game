#include "scene_menu.h"

#include "graphics.h"
#include "game.h"

#include <GL/glew.h>

CSceneMenu::CSceneMenu() {}

CSceneMenu::~CSceneMenu() {
  glDeleteBuffers(1, &m_backgroundBuffer);
}

void CSceneMenu::OnInit() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glGenBuffers(1, &m_backgroundBuffer);

  vertex_t vertices[] = {
    { 0.0f, 1.0f, 0.0f, 0.5f, 0.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f }
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrthof(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}

void CSceneMenu::OnRender() {
  CGame::Inst->GetGraphics()->Begin();

  glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  CGame::Inst->GetGraphics()->End();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}
