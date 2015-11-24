#pragma once

#include "scene.hpp"

class CSceneGamemode : public CScene {
public:
  CSceneGamemode();

  virtual unsigned short GetGamemodeID() = 0;
};
