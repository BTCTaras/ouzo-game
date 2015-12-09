#include "scene/scene_osu.hpp"

#include "game.hpp"
#include "graphics_common/graphics.hpp"
#include "graphics_common/sprite_verts.hpp"

#include <algorithm>

///
/// CSceneOsu
///

void CSceneOsu::OnInit() {
	m_backgroundBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	m_backgroundBuffer->Orphan(SPRITE_VERTS_SIZE, sizeof(vertex_t), (void*)SPRITE_VERTS);

	m_mvpMatrix.projection = GFX->CreateIdentityMatrix();
	m_mvpMatrix.view = GFX->CreateIdentityMatrix();
	m_mvpMatrix.model = GFX->CreateIdentityMatrix();
}

void CSceneOsu::OnResize(int width, int height) {
	m_mvpMatrix.projection->LoadIdentity();
	m_mvpMatrix.projection->Orthographic(0, (float)width, (float)height, 0);
}

void CSceneOsu::SetBackgroundTex(S_CTexture tex) {
	m_backgroundTex = tex;
}

unsigned short CSceneOsu::GetGamemodeID() {
	return (unsigned short)0x0;
}

void CSceneOsu::OnRender() {
	GFX->SetDrawProgram(GFX->GetDefaultProgram());

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(0.0f, 0.0f, -0.9f);
	m_mvpMatrix.model->Scale((float)m_backgroundTex->GetWidth(), (float)m_backgroundTex->GetHeight(), 1.0f);
	
	static S_CDrawAttribs bgattribs = nullptr;

	if (bgattribs == nullptr) {
		bgattribs = GFX->CreateDrawAttribs(m_backgroundBuffer);
	}

	GFX->SetDrawBuffer(m_backgroundBuffer);
	GFX->SetDrawTexture(m_backgroundTex);
	GFX->SetDrawTransform(m_mvpMatrix);
	GFX->SetDrawAttributes(bgattribs);

	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);

	m_beatmap.OnRender(m_mvpMatrix);
}