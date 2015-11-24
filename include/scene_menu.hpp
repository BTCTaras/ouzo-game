#pragma once

#include "ui.hpp"
#include "shader.hpp"
#include "graphics.hpp"
#include "texture.hpp"
#include "font.hpp"
#include "text.hpp"

class CSceneMenu : public CSceneUI {
public:
  CSceneMenu();
  ~CSceneMenu();

  virtual void OnUpdate();
  virtual void OnLeave(S_CScene *scene);
  virtual void OnRender();
  virtual void OnInitUI();

  virtual CTexture* GetBackgroundTexture();

private:
  CTexture m_buttonTexture;
  CTexture m_texture;

  S_CProgram m_fontProgram;
  CFont m_font;
  CText m_helloWorld;
};
