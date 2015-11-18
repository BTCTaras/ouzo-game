#pragma once

#include "scene.h"

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
};
