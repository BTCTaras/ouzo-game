#include "scene_menu.h"

#include "graphics.h"
#include "game.h"
#include "shader.h"
#include "font.h"
#include "text.h"
#include "ui.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

CSceneMenu::CSceneMenu() {}
CSceneMenu::~CSceneMenu() {}

void CSceneMenu::OnInitUI() {
  m_buttonTexture.LoadFromFile("assets/ui/button_test.png");
  m_font.LoadFromFile("assets/fonts/Lato-Regular.ttf");

  S_CUIControlTexture tex(new CUIControlTexture(&m_buttonTexture));
  tex->x = 80;
  tex->y = 40;
  tex->z = -0.8f;
  this->AddControl(tex);

  S_CUIControlText text(new CUIControlText(&m_font, U"butt:)", 32));
  text->x = 200;
  text->y = 100;
  text->z = 0.8f;
  this->AddControl(text);

  m_texture.LoadFromFile("assets/backgrounds/test.jpeg");

  // We're done compiling shaders now.
  if (glReleaseShaderCompiler) {
    glReleaseShaderCompiler();
  }
}

void CSceneMenu::OnRender() {
  CSceneUI::OnRender();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}

CTexture* CSceneMenu::GetBackgroundTexture() {
  return &m_texture;
}
