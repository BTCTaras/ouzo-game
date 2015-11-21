#include "scene_menu.h"

#include "graphics.h"
#include "game.h"
#include "shader.h"
#include "font.h"

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

  S_CShader shaders[] = {
    S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/uitext.vsh")),
    S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/uitext.fsh")),
  };

  m_fontProgram.reset(new CProgram(2, shaders));

  glEnable(GL_TEXTURE_3D);

  CFont font("assets/fonts/Ubuntu-C.ttf");

  unsigned long chars[26 * 2];

  // Latin alphabet
  for (unsigned long i = 0x41; i < 0x41 + 26 * 2; ++i) {
    chars[i - 0x41] = i;
  }

  font.CreateGlyphTexture(&m_fontTexture, 128, 3, chars);

  // We're done compiling shaders now.
  if (glReleaseShaderCompiler) {
    glReleaseShaderCompiler();
  }
}

void CSceneMenu::OnRender() {
  CSceneUI::OnRender();

  m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 300.0f, 0.0f));
  m_mvpMatrix.model = glm::scale(m_mvpMatrix.model, glm::vec3(384.0f, 128.0f, 1.0f));

  m_fontTexture.Use();
  GFX->Begin(m_mvpMatrix, m_fontProgram);

  glUniform1i(m_fontProgram->GetUniformLocation("u_Char", true), 0);
  glUniform3f(m_fontProgram->GetUniformLocation("u_FontColour", true), 0.0f, 1.0f, 1.0f);

  glBindBuffer(GL_ARRAY_BUFFER, CUISprite::s_globalSpriteBuffer);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  GFX->End();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}

CTexture* CSceneMenu::GetBackgroundTexture() {
  return &m_texture;
}
