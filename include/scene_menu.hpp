#pragma once

#include "ui.hpp"
#include "shader.hpp"
#include "graphics.hpp"
#include "texture.hpp"
#include "font.hpp"
#include "text.hpp"
#include "buffer.hpp"

class CSceneMenu : public CSceneUI {
public:
  CSceneMenu();
  ~CSceneMenu();

  virtual void OnUpdate() override;
  virtual void OnLeave(CScene *scene) override;
  virtual void OnRender() override;
  virtual void OnInitUI() override;

  virtual S_CTexture GetBackgroundTexture() override;

private:
  S_CTexture m_buttonTexture;
  S_CTexture m_texture;

  S_CProgram m_fontProgram;
  CFont m_font;
  CText m_helloWorld;

  S_CBuffer m_testBuffer;
};
