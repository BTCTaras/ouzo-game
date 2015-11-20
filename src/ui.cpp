#include "ui.h"

#include <GL/glew.h>

#include "graphics.h"
#include "texture.h"
#include "game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CSceneUI::CSceneUI() {

}

CSceneUI::~CSceneUI() {
  glDeleteBuffers(1, &m_backgroundBuffer);
}

void CSceneUI::OnInit() {
  glGenBuffers(1, &m_backgroundBuffer);

  const vertex_t vertices[] = {
    { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f }
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  m_mvpMatrix.projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  m_mvpMatrix.view = glm::mat4(1.0f);
  m_mvpMatrix.model = glm::mat4(1.0f);
}

void CSceneUI::AddControl(S_CUIControl control) {
  m_controls.push_back(control);
}

void CSceneUI::OnRender() {
  CTexture *tex = this->GetBackgroundTexture();

  if (tex != nullptr) {
    GFX->Begin(m_mvpMatrix, GFX->GetDefaultProgram());
    tex->Use();

    glBindBuffer(GL_ARRAY_BUFFER, m_backgroundBuffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    GFX->End();
  }

  for (S_CUIControl control : m_controls) {
    control->Render();
  }
}

CTexture* CSceneUI::GetBackgroundTexture() {
  return nullptr;
}
