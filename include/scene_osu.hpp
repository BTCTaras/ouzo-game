#include "scene_gamemode.hpp"

class CSceneOsu : public CSceneGamemode {
  virtual unsigned short GetGamemodeID() override;
  virtual void OnRender();
};
