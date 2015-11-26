#include "ui.hpp"

#include <GL/glew.h>

#include "graphics.hpp"
#include "texture.hpp"
#include "game.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const vertex_t QUAD_VERTICES[] = {
  { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
  { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
  { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
};

///
/// CSceneUI
///

CSceneUI::CSceneUI() {
	m_mainControl.reset(new CUIControlTexture);
	m_mainControl->z = -1.0f;
}

CSceneUI::~CSceneUI() {}

void CSceneUI::OnInit() {
	m_mvpMatrix.projection = glm::mat4(1.0f);
	m_mvpMatrix.view = glm::mat4(1.0f);
	m_mvpMatrix.model = glm::mat4(1.0f);

	this->OnInitUI();
}

void CSceneUI::AddControl(std::shared_ptr<CUIControl> control) {
	m_mainControl->AddChild(control);
}

void CSceneUI::OnResize(int width, int height) {
	m_width = width;
	m_height = height;
	m_mvpMatrix.projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	this->UpdateBackground();
}

void CSceneUI::UpdateBackground() {
	m_mainControl->SetTexture(this->GetBackgroundTexture());
	m_mainControl->width = (float)m_width;
	m_mainControl->height = (float)m_height;
}

void CSceneUI::OnRender() {
	for (S_CUIRenderable renderable : *m_mainControl->GetRenderables()) {
		m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(m_mainControl->x, m_mainControl->y, m_mainControl->z));
		renderable->OnRender(m_mvpMatrix);
	}

	for (S_CUIControl control : *m_mainControl->GetChildren()) {
		for (S_CUIRenderable renderable : *control->GetRenderables()) {
			m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(control->x, control->y, control->z));
			renderable->OnRender(m_mvpMatrix);  // This is pretty naive.
												// Replace with instancing (& multidraw indirect).
		}
	}
}

S_CTexture CSceneUI::GetBackgroundTexture() {
	return nullptr;
}

void CSceneUI::OnClick(unsigned int button, float x, float y) {
	printf("Clicked at %d, %d with button %d\n", (int)x, (int)y, button);

	for (S_CUIControl c : *m_mainControl->GetChildren()) {
		if (x <= c->x + c->width && x >= c->x && y <= c->y + c->height && y >= c->y) {
			ui_event_params_t clickEvent;
			clickEvent.x = x - c->x;
			clickEvent.y = y - c->y;
			clickEvent.button = button;

			UIEvent type = UIEvent::CLICK;
			c->HandleEvent(type, clickEvent);
		}
	}
}

///
/// CUIControl
///

CUIControl::CUIControl()
	: x(0.0f), y(0.0f), z(0.0f),
	width(0.0f), height(0.0f)
{
}

void CUIControl::AddChild(std::shared_ptr<CUIControl> control) {
	m_children.push_back(control);
}

void CUIControl::SetPosition(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void CUIControl::SetSize(float width, float height) {
	this->width = width;
	this->height = height;
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

S_CBuffer CUISprite::s_globalSpriteBuffer = nullptr;

CUISprite::CUISprite(S_CTexture tex) {
	if (tex != nullptr) {
		this->SetTexture(tex);
	}

	if (s_globalSpriteBuffer == nullptr) {
		s_globalSpriteBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
		s_globalSpriteBuffer->Orphan(sizeof(QUAD_VERTICES), (void*)QUAD_VERTICES);
	}
}

void CUISprite::OnRender(mvp_matrix_t &mvp) {
	mvp.model = glm::scale(mvp.model, glm::vec3(m_texture->GetWidth(), m_texture->GetHeight(), 1.0f));

	GFX->Begin(mvp, s_globalSpriteBuffer);
	GFX->SetTexture(m_texture);
	GFX->Draw(PrimitiveType::TRIANGLE_STRIP);
	GFX->End();
}

void CUISprite::SetTexture(S_CTexture tex) {
	m_texture = tex;
}

///
/// CUIText
///
CUIText::CUIText(CText *text)
	: m_text(text)
{
}

void CUIText::SetText(const std::u32string &text) {
	m_text->SetText(text);
}

void CUIText::OnRender(mvp_matrix_t &mvp) {
	m_text->Render(mvp);
}

///
/// CUIControl
///
void CUIControl::HandleEvent(UIEvent event, ui_event_params_t &params) {
	if (event == UIEvent::CLICK) {
		printf("Got clicked at %f, %f with button %d\n", params.x, params.y, params.button);
	}
}

///
/// CUIControlText
///
CUIControlText::CUIControlText(CFont *font, const std::u32string &text, unsigned int size)
	: m_text(font, size, text),
	m_uiText(new CUIText(&m_text))
{
	this->AddRenderable(m_uiText);
}

void CUIControlText::SetText(const std::u32string &text) {
	m_uiText->SetText(text);
}

///
/// CUIControlTexture
///

CUIControlTexture::CUIControlTexture(S_CTexture tex)
	: m_sprite(new CUISprite(nullptr))
{
	if (tex != nullptr) {
		m_sprite->SetTexture(tex);
		this->width = (float)tex->GetWidth();
		this->height = (float)tex->GetHeight();
	}

	this->AddRenderable(m_sprite);
}

void CUIControlTexture::SetTexture(S_CTexture tex) {
	m_sprite->SetTexture(tex);
}
