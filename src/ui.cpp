#include "ui.h"

#include <GL/glew.h>

#include "graphics.h"
#include "texture.h"
#include "game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const vertex_t QUAD_VERTICES[] = {
  { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
  { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
  { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f }
};

///
/// CSceneUI
///

CSceneUI::CSceneUI() {}
CSceneUI::~CSceneUI() {}

void CSceneUI::OnInit() {
  m_mvpMatrix.projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  m_mvpMatrix.view = glm::mat4(1.0f);
  m_mvpMatrix.model = glm::mat4(1.0f);

  m_mainControl.reset(new CUIControlBackground(this->GetBackgroundTexture()));
}

void CSceneUI::AddControl(S_CUIControl control) {
  m_mainControl->AddChild(control);
}

void CSceneUI::OnRender() {
  for (S_CUIRenderable renderable : *m_mainControl->GetRenderables()) {
    renderable->OnRender(m_mvpMatrix);
  }
  
  for (S_CUIControl control : *m_mainControl->GetChildren()) {
    for (S_CUIRenderable renderable : *m_mainControl->GetRenderables()) {
      renderable->OnRender(m_mvpMatrix);  // This is pretty naive.
                                          // Replace with instancing (& multidraw indirect).
    }
  }
}

CTexture* CSceneUI::GetBackgroundTexture() {
  return nullptr;
}

///
/// CUIControl
///

void CUIControl::AddChild(S_CUIControl control) {
  m_children.push_back(control);
}

void CUIControl::AddRenderable(S_CUIRenderable renderable) {
  m_renderables.push_back(renderable);
}

std::vector< std::shared_ptr<CUIControl> >* CUIControl::GetChildren() {
  return &m_children;
}

std::vector<S_CUIRenderable>* CUIControl::GetRenderables() {
  return &m_renderables;
}

///
/// CUISprite
///

unsigned int CUISprite::s_globalSpriteBuffer = 0;
unsigned int CUISprite::s_activeSprites = 0;

CUISprite::CUISprite(CTexture *tex)
  : m_texture(tex)
{
  if (s_globalSpriteBuffer == 0) {
    glGenBuffers(1, &s_globalSpriteBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, s_globalSpriteBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
  }

  s_activeSprites++;
}

CUISprite::~CUISprite() {
  s_activeSprites--;

  if (s_activeSprites == 0) {
    glDeleteBuffers(1, &s_globalSpriteBuffer);
    s_globalSpriteBuffer = 0;
  }
}

void CUISprite::OnRender(mvp_matrix_t &mvp) {
  GFX->Begin(mvp, GFX->GetDefaultProgram());
  m_texture->Use();

  glBindBuffer(GL_ARRAY_BUFFER, s_globalSpriteBuffer);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  GFX->End();
}

///
/// CUIControl
///

void CUIControl::HandleEvent(UIEvent &event) {

}

///
/// CUIControlBackground
///

CUIControlBackground::CUIControlBackground(CTexture *tex) {
  this->AddRenderable(S_CUISprite(new CUISprite(tex)));
}
