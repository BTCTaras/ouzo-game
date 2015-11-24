#include "scene_gamemode.h"

class CSceneOsu : public CSceneGamemode {
  virtual unsigned short GetGamemodeID() override;
  virtual void OnRender();
};
