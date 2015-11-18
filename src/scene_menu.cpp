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

CSceneMenu::~CSceneMenu() {
  glDeleteBuffers(1, &m_backgroundBuffer);
}

void CSceneMenu::OnInit() {
  glClearColor(0.0, 0.0, 0.0, 0.0);

  m_program.reset(new CProgram);

  S_CShader shaders[] = {
    S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/diffuse.vsh")),
    S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/diffuse.fsh"))
  };

  m_program->LoadFromShaders(2, shaders);

  glGenBuffers(1, &m_backgroundBuffer);

  vertex_t vertices[] = {
    { 0.0f, 1.0f, 0.0f, 0.5f, 0.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f }
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  m_mvpMatrix.projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  m_mvpMatrix.view = glm::mat4(1.0f);
  m_mvpMatrix.model = glm::mat4(1.0f);

  // Tell OpenGL that we're done with compiling shaders.
  // Does not disable shader compilation, but shader compilation
  // will take more time to warm up in the future!
  if (glReleaseShaderCompiler) {
    glReleaseShaderCompiler();
  }
}

void CSceneMenu::OnRender() {
  CGame::Inst->GetGraphics()->Begin(m_mvpMatrix, m_program);

  glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  CGame::Inst->GetGraphics()->End();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}
