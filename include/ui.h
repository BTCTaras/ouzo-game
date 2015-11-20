#pragma once

#include "scene.h"
#include "graphics.h"
#include "shader.h"
#include "texture.h"

#include <memory>
#include <vector>

enum UIEvent {
  CLICK
};

struct ui_colour_t {
  float r, g, b;
};

class CUIControl {
public:
  virtual void Render() = 0;
  virtual void Update() = 0;
  virtual void HandleEvent(UIEvent &event);
};

typedef std::shared_ptr<CUIControl> S_CUIControl;

class CSceneUI : public CScene {
public:
  CSceneUI();
  virtual ~CSceneUI();

  virtual void OnInit();

  void AddControl(S_CUIControl control);

  virtual void OnRender();
  virtual CTexture* GetBackgroundTexture();

private:
  std::vector<S_CUIControl> m_controls;
  unsigned int m_backgroundBuffer;

  mvp_matrix_t m_mvpMatrix;
  S_CProgram m_program;

};
