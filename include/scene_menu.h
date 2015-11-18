#pragma once

#include "scene.h"
#include "shader.h"
#include "graphics.h"

class CSceneMenu : public CScene {
public:
  CSceneMenu();
  ~CSceneMenu();

  virtual void OnInit();
  virtual void OnRender();
  virtual void OnUpdate();
  virtual void OnLeave(S_CScene *scene);

private:
  unsigned int m_backgroundBuffer;

  mvp_matrix_t m_mvpMatrix;
  S_CProgram m_program;
};
