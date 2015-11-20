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

struct ui_event_params_t {
  float x, y;
  unsigned int button;
};

struct ui_colour_t {
  float r, g, b;
};

class CUIRenderable {
public:
  virtual void OnRender(mvp_matrix_t &mvp) = 0;
};

typedef std::shared_ptr<CUIRenderable> S_CUIRenderable;

class CUISprite : public CUIRenderable {
public:
  CUISprite(CTexture *tex = nullptr);
  ~CUISprite();

  void SetTexture(CTexture *tex);

  virtual void OnRender(mvp_matrix_t &mvp);

private:
  CTexture *m_texture;

  static unsigned int s_globalSpriteBuffer;
  static unsigned int s_activeSprites;
};

typedef std::shared_ptr<CUISprite> S_CUISprite;

class CUIControl {
public:
  CUIControl();
  virtual void HandleEvent(UIEvent event, ui_event_params_t &params);

  void AddChild(std::shared_ptr<CUIControl> control);
  std::vector< std::shared_ptr<CUIControl> >* GetChildren();
  std::vector<S_CUIRenderable>* GetRenderables();

  float x, y, width, height;

protected:
  void AddRenderable(S_CUIRenderable renderable);

private:
  std::vector< std::shared_ptr<CUIControl> > m_children;
  std::vector<S_CUIRenderable> m_renderables;
};

typedef std::shared_ptr<CUIControl> S_CUIControl;

class CUIControlTexture : public CUIControl {
public:
  CUIControlTexture(CTexture *texture = nullptr);

  void SetTexture(CTexture *texture);

private:
  S_CUISprite m_sprite;
};

typedef std::shared_ptr<CUIControlTexture> S_CUIControlTexture;

class CSceneUI : public CScene {
public:
  CSceneUI();
  virtual ~CSceneUI();

  virtual void OnInit();

  void AddControl(std::shared_ptr<CUIControl> control);

  virtual void OnRender();
  virtual CTexture* GetBackgroundTexture();

  void UpdateBackground();

  virtual void OnResize(int width, int height);
  virtual void OnClick(unsigned int button, float width, float height);

private:
  S_CUIControlTexture m_mainControl;

  mvp_matrix_t m_mvpMatrix;
  S_CProgram m_program;

  int m_width, m_height;

};
