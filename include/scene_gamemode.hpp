#pragma once

#include "scene.h"

class CSceneGamemode : public CScene {
public:
  CSceneGamemode();

  virtual unsigned int GetGamemodeID() = 0;
};
