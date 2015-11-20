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

class CUIRenderable {
public:
  virtual void OnRender(mvp_matrix_t &mvp) = 0;
};

typedef std::shared_ptr<CUIRenderable> S_CUIRenderable;

class CUISprite : public CUIRenderable {
public:
  CUISprite(CTexture *tex);
  ~CUISprite();

  virtual void OnRender(mvp_matrix_t &mvp);

private:
  CTexture *m_texture;

  static unsigned int s_globalSpriteBuffer;
  static unsigned int s_activeSprites;
};

typedef std::shared_ptr<CUISprite> S_CUISprite;

class CUIControl {
public:
  virtual void HandleEvent(UIEvent &event);

  void AddChild(std::shared_ptr<CUIControl> control);
  std::vector< std::shared_ptr<CUIControl> >* GetChildren();
  std::vector<S_CUIRenderable>* GetRenderables();

protected:
  void AddRenderable(S_CUIRenderable renderable);

private:
  std::vector< std::shared_ptr<CUIControl> > m_children;
  std::vector<S_CUIRenderable> m_renderables;
};

class CUIControlBackground : public CUIControl {
public:
  CUIControlBackground(CTexture *texture);
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
  S_CUIControl m_mainControl;

  mvp_matrix_t m_mvpMatrix;
  S_CProgram m_program;

};
