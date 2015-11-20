#include "scene_menu.h"

#include "graphics.h"
#include "game.h"
#include "shader.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

CSceneMenu::CSceneMenu() {}
CSceneMenu::~CSceneMenu() {}

void CSceneMenu::OnInit() {
  CSceneUI::OnInit();

  m_texture.LoadFromFile("assets/backgrounds/test.jpeg");

  // Tell OpenGL that we're done with compiling shaders.
  // Does not disable shader compilation, but shader compilation
  // will take more time to warm up in the future!
  if (glReleaseShaderCompiler) {
    glReleaseShaderCompiler();
  }
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}

CTexture* CSceneMenu::GetBackgroundTexture() {
  return &m_texture;
}
