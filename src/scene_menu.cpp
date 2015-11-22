#include "scene_menu.h"

#include "graphics.h"
#include "game.h"
#include "shader.h"
#include "font.h"
#include "text.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

CSceneMenu::CSceneMenu() {}
CSceneMenu::~CSceneMenu() {}

void CSceneMenu::OnInit() {
  m_buttonTexture.LoadFromFile("assets/ui/button_test.png");

  S_CUIControlTexture tex(new CUIControlTexture(&m_buttonTexture));
  tex->x = 80;
  tex->y = 40;
  this->AddControl(tex);

  CSceneUI::OnInit();

  m_texture.LoadFromFile("assets/backgrounds/test.jpeg");

  m_font.LoadFromFile("assets/fonts/Ubuntu-C.ttf");
  m_helloWorld.InitText(&m_font, 128, U"Hello World!");

  // We're done compiling shaders now.
  if (glReleaseShaderCompiler) {
    glReleaseShaderCompiler();
  }
}

void CSceneMenu::OnRender() {
  CSceneUI::OnRender();

  m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 300.0f, 0.0f));
  m_helloWorld.Render(m_mvpMatrix);
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}

CTexture* CSceneMenu::GetBackgroundTexture() {
  return &m_texture;
}
