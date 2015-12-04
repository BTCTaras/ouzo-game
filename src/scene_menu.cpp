#include "scene_menu.hpp"

#include "graphics.hpp"
#include "game.hpp"
#include "shader.hpp"
#include "font.hpp"
#include "text.hpp"
#include "buffer.hpp"

#include "ui.hpp"
#include "uic_text.hpp"
#include "uic_texture.hpp"

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
	tex->SetPosition(80, 40, -0.8f);
	this->AddControl(tex);

	S_CUIControlText text(new CUIControlText(&m_font, u8"Test Button", 32));
	text->SetPosition(200, 100, 0.8f);
	//this->AddControl(text);

	m_texture = GFX->CreateTexture("assets/backgrounds/test.jpeg");

	m_testBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);

	vertex_t vertices[] = {
	  { 64.0f, 0.0f, -0.8f, 0.5f, 0.0f },
	  { 0.0f, 128.0f, -0.8f, 0.0f, 1.0f },
	  { 128.0f, 128.0f, -0.8f, 1.0f, 1.0f },
	};

	m_testBuffer->Orphan(sizeof(vertices), sizeof(vertex_t), vertices);
}

void CSceneMenu::OnRender() {
	CSceneUI::OnRender();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(CScene *scene) {

}

S_CTexture CSceneMenu::GetBackgroundTexture() {
	return m_texture;
}
