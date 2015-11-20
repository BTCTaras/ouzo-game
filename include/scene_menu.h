#pragma once

#include "ui.h"
#include "shader.h"
#include "graphics.h"
#include "texture.h"

class CSceneMenu : public CSceneUI {
public:
  CSceneMenu();
  ~CSceneMenu();

  virtual void OnInit();
  virtual void OnUpdate();
  virtual void OnLeave(S_CScene *scene);

  virtual CTexture* GetBackgroundTexture();

private:
  CTexture m_texture;
};
