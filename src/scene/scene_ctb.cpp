#include "scene/scene_ctb.hpp"

#include "graphics_common/graphics.hpp"
#include "graphics_common/sprite_verts.hpp"

#include "game.hpp"

#include <math.h>
#include <algorithm>

S_CBuffer g_circleBuffer = nullptr;

CSceneCTB::CSceneCTB()
	: m_dudeX(0), m_dudeY(0)
{
}

void CSceneCTB::OnInit() {
	if (g_circleBuffer == nullptr) {
		g_circleBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
		g_circleBuffer->Orphan(SPRITE_VERTS_SIZE, sizeof(vertex_t), (void*)SPRITE_VERTS);
	}

	m_quadAttribs = GFX->CreateDrawAttribs(g_circleBuffer);

	m_fruitTex = GFX->CreateTexture("assets/sickfruit.png");
	m_bowlTex = GFX->CreateTexture("assets/bowl.png");

	m_fruit.push_back({ 50.0f, 50.0f });
}

void CSceneCTB::OnResize(int width, int height) {
	m_mvpMatrix.projection = GFX->CreateIdentityMatrix();
	m_mvpMatrix.projection->Orthographic(0.0f, (float)width, (float)height, 0.0f);
	m_mvpMatrix.model = GFX->CreateIdentityMatrix();
	m_mvpMatrix.view = GFX->CreateIdentityMatrix();

	m_dudeY = height - 64.0f;
}

void CSceneCTB::OnRender() {
	GFX->SetDrawProgram(GFX->GetDefaultProgram());
	GFX->SetDrawAttributes(m_quadAttribs);
	GFX->SetDrawBuffer(g_circleBuffer);

	for (fruit_t &fruit : m_fruit) {
		m_mvpMatrix.model->LoadIdentity();
		m_mvpMatrix.model->Translate(fruit.x, fruit.y);
		m_mvpMatrix.model->Scale(64.0f, 64.0f, 1.0f);

		GFX->SetDrawTransform(m_mvpMatrix);
		GFX->SetDrawTexture(m_fruitTex);
		GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
	}

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(m_dudeX, m_dudeY);
	m_mvpMatrix.model->Scale(128.0f, 64.0f, 1.0f);

	GFX->SetDrawTransform(m_mvpMatrix);
	GFX->SetDrawTexture(m_bowlTex);
	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
}

void CSceneCTB::OnUpdate() {
	std::vector<fruit_t*> toRemove;

	for (fruit_t &fruit : m_fruit) {
		fruit.y += CGame::Inst->GetDeltaTime() * 100.0f;

		if ( (fruit.x > m_dudeX && fruit.x < m_dudeX + 128.0f && fruit.y > m_dudeY)
			|| fruit.y > CGame::Inst->GetHeight() + fruit.y ) {
			toRemove.push_back(&fruit);
		}
	}

	for (fruit_t *fruit : toRemove) {
		std::remove_if(m_fruit.begin(), m_fruit.end(), [=](fruit_t &f) {
			return f.x == fruit->x && f.y == fruit->y;
		});
	}

	static float fruitTimer = 0.0f;
	fruitTimer += (float)CGame::Inst->GetDeltaTime();

	printf("%f\n", fruitTimer);

	if ((int)(fruitTimer) % 2 == 0) {
		m_fruit.push_back({ (float)(rand() % CGame::Inst->GetWidth()), 0 });
	}

	// shit no keyboard api yet :((
}

void CSceneCTB::OnClick(unsigned int button, float x, float y) {
	if (button == 0) { // left click
		m_dudeX = x - 64.0f;
	}
}

unsigned short CSceneCTB::GetGamemodeID() {
	return 0x1;
}
