#pragma once

#include "scene.hpp"

class CSceneGamemode : public CScene {
public:
  CSceneGamemode();

  virtual unsigned int GetGamemodeID() = 0;
};
