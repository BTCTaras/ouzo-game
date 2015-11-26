#pragma once

#include "scene.hpp"
#include "graphics.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "text.hpp"
#include "buffer.hpp"

#include <memory>
#include <vector>
#include <string>

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
  CUISprite(S_CTexture tex = nullptr);

  void SetTexture(S_CTexture tex);

  virtual void OnRender(mvp_matrix_t &mvp);

private:
  S_CTexture m_texture;
  static S_CBuffer s_globalSpriteBuffer;
};

typedef std::shared_ptr<CUISprite> S_CUISprite;

class CUIText : public CUIRenderable {
public:
  CUIText(CText *text);

  void SetText(const std::u32string &text);

  virtual void OnRender(mvp_matrix_t &mvp);

private:
  CText *m_text;
};

typedef std::shared_ptr<CUIText> S_CUIText;

class CUIControl {
public:
  CUIControl();
  virtual void HandleEvent(UIEvent event, ui_event_params_t &params);

  void AddChild(std::shared_ptr<CUIControl> control);
  std::vector< std::shared_ptr<CUIControl> >* GetChildren();
  std::vector<S_CUIRenderable>* GetRenderables();

  float x, y, z, width, height;

protected:
  void AddRenderable(S_CUIRenderable renderable);

private:
  std::vector< std::shared_ptr<CUIControl> > m_children;
  std::vector<S_CUIRenderable> m_renderables;
};

typedef std::shared_ptr<CUIControl> S_CUIControl;

class CUIControlTexture : public CUIControl {
public:
  CUIControlTexture(S_CTexture texture = nullptr);

  void SetTexture(S_CTexture texture);

private:
  S_CUISprite m_sprite;
};

typedef std::shared_ptr<CUIControlTexture> S_CUIControlTexture;

class CUIControlText : public CUIControl {
public:
  CUIControlText(CFont *font, const std::u32string &text, unsigned int size);

  void SetText(const std::u32string &text);

private:
  CText m_text;
  S_CUIText m_uiText;
};

typedef std::shared_ptr<CUIControlText> S_CUIControlText;

class CSceneUI : public CScene {
public:
  CSceneUI();
  virtual ~CSceneUI();

  virtual void OnInit();
  virtual void OnInitUI() = 0;

  void AddControl(std::shared_ptr<CUIControl> control);

  virtual void OnRender();
  virtual S_CTexture GetBackgroundTexture();

  void UpdateBackground();

  virtual void OnResize(int width, int height);
  virtual void OnClick(unsigned int button, float width, float height);

protected:
  mvp_matrix_t m_mvpMatrix;

private:
  S_CUIControlTexture m_mainControl;
  S_CProgram m_program;

  int m_width, m_height;

};
