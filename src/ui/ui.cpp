#include "ui/ui.hpp"

#include <GL/glew.h>

#include "graphics_common/graphics.hpp"
#include "graphics_common/texture.hpp"
#include "graphics_common/sprite_verts.hpp"

#include "game.hpp"
#include "ui/uic_texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

///
/// CSceneUI
///

CSceneUI::CSceneUI() {
	m_mainControl.reset(new CUIControlTexture);
	m_mainControl->z = -1.0f;
}

CSceneUI::~CSceneUI() {}

void CSceneUI::OnInit() {
	m_mvpMatrix.projection = GFX->CreateIdentityMatrix();
	m_mvpMatrix.view = GFX->CreateIdentityMatrix();
	m_mvpMatrix.model = GFX->CreateIdentityMatrix();

	this->OnInitUI();
}

void CSceneUI::AddControl(S_CUIControl control) {
	m_mainControl->AddChild(control);
}

void CSceneUI::OnResize(int width, int height) {
	CScene::OnResize(width, height);

	m_width = width;
	m_height = height;
	m_mvpMatrix.projection->Orthographic(0.0f, (float)width, (float)height, 0.0f);
	this->UpdateBackground();
}

void CSceneUI::UpdateBackground() {
	m_mainControl->SetTexture(this->GetBackgroundTexture());
	m_mainControl->width = (float)m_width;
	m_mainControl->height = (float)m_height;
}

void CSceneUI::OnRender() {
	RenderControl(m_mainControl, m_mvpMatrix);
}

void CSceneUI::RenderControl(S_CUIControl control, mvp_matrix_t &matrix) {
	for (S_CUIRenderable renderable : *control->GetRenderables()) {
		matrix.model->LoadIdentity();
		matrix.model->Translate(control->x, control->y, control->z);

		renderable->OnRender(matrix);
	}

	for (S_CUIControl child : *control->GetChildren()) {
		this->RenderControl(child, matrix);
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
	:	x(0.0f), y(0.0f), z(0.0f),
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
S_CDrawAttribs CUISprite::s_globalSpriteDrawAttribs = nullptr;

CUISprite::CUISprite(S_CTexture tex) {
	if (tex != nullptr) {
		this->SetTexture(tex);
	}

	if (s_globalSpriteBuffer == nullptr) {
		s_globalSpriteBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
		s_globalSpriteBuffer->Orphan(SPRITE_VERTS_SIZE, sizeof(vertex_t), (void*)SPRITE_VERTS);

		s_globalSpriteDrawAttribs = GFX->CreateDrawAttribs(s_globalSpriteBuffer);
	}
}

void CUISprite::OnRender(mvp_matrix_t &mvp) {
	mvp.model->Scale(
		(float)m_texture->GetWidth(),
		(float)m_texture->GetHeight(),
		1.0f
	);

	GFX->SetDrawProgram(GFX->GetDefaultProgram());
	GFX->SetDrawTransform(mvp);
	GFX->SetDrawAttributes(s_globalSpriteDrawAttribs);
	GFX->SetDrawBuffer(s_globalSpriteBuffer);
	GFX->SetDrawTexture(m_texture);

	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
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

void CUIText::SetText(const std::string &text) {
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