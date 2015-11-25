#include "scene_menu.hpp"

#include "graphics.hpp"
#include "game.hpp"
#include "shader.hpp"
#include "font.hpp"
#include "text.hpp"
#include "ui.hpp"
#include "buffer.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CSceneMenu::CSceneMenu() {}
CSceneMenu::~CSceneMenu() {}

void CSceneMenu::OnInitUI() {
  m_buttonTexture = GFX->CreateTexture("assets/ui/button_test.png");
  m_font.LoadFromFile("assets/fonts/Lato-Regular.ttf");

  S_CUIControlTexture tex(new CUIControlTexture(m_buttonTexture));
  tex->x = 80;
  tex->y = 40;
  tex->z = -0.8f;
  this->AddControl(tex);

  S_CUIControlText text(new CUIControlText(&m_font, U"butt:)", 32));
  text->x = 200;
  text->y = 100;
  text->z = 0.8f;
  this->AddControl(text);

  m_texture = GFX->CreateTexture("assets/backgrounds/test.jpeg");

  m_testBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);

  vertex_t vertices[] = {
    { 64.0f, 0.0f, 0.9f, 0.5f, 0.0f },
    { 0.0f, 128.0f, 0.9f, 0.0f, 1.0f },
    { 128.0f, 128.0f, 0.9f, 1.0f, 1.0f },
  };

  m_testBuffer->Orphan(sizeof(vertices), vertices);
}

void CSceneMenu::OnRender() {
  CSceneUI::OnRender();

  // Set the position of our triangle
  m_mvpMatrix.model = glm::translate(
    glm::mat4(1.0f), // Identity matrix (no transformation)
    glm::vec3(128.0f, 128.0f, 0.0f) // Translate the identity by this vector
  );

  GFX->Begin(m_mvpMatrix); // Begin with our MVP matrix

  GFX->SetTexture(nullptr, 0); // No texture in slot 0
  GFX->Draw(
    PrimitiveType::TRIANGLES, // Connect our vertices to form triangles
    m_testBuffer // Draw our triangle buffer
  );

  GFX->End();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(CScene *scene) {

}

S_CTexture CSceneMenu::GetBackgroundTexture() {
  return m_texture;
}
