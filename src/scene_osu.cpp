#include "scene_osu.hpp"

#include "game.hpp"
#include "graphics.hpp"

#include <algorithm>

///
/// CSceneOsu
///

const vertex_t CIRCLE_VERTS[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
};

CSceneOsu::CSceneOsu() {
	m_circleBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	m_circleBuffer->Orphan(sizeof(CIRCLE_VERTS), (void*)CIRCLE_VERTS);

	m_mvpMatrix.projection = GFX->CreateIdentityMatrix();
	m_mvpMatrix.view = GFX->CreateIdentityMatrix();
	m_mvpMatrix.model = GFX->CreateIdentityMatrix();

	m_circleTex = GFX->CreateTexture("assets/gamemodes/standard/circle.png");
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
	// Simple state sorting
	std::sort(m_objects.begin(), m_objects.end(), [](S_COsuObject a, S_COsuObject b) {
		return a->GetType() < b->GetType();
	});

	for (S_COsuObject obj : m_objects) {
		switch (obj->GetType()) {
		case OsuObjectType::CIRCLE:
			this->RenderCircle(obj->x, obj->y, NULL);
			break;

		case OsuObjectType::SLIDER:
			this->RenderSlider(obj->x, obj->y, NULL);
			break;

		case OsuObjectType::SPINNER:
			this->RenderSpinner(obj->x, obj->y, NULL);
			break;
		}
	}

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(0.0f, 0.0f, -1.0f);
	m_mvpMatrix.model->Scale((float)m_backgroundTex->GetWidth(), (float)m_backgroundTex->GetHeight(), 1.0f);
	
	GFX->Begin(m_mvpMatrix, m_circleBuffer);
	GFX->SetTexture(m_backgroundTex);
	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
	GFX->End();
}

unsigned int CSceneOsu::MakeUniqueObjectID() {
	return m_nextUID++;
}

void CSceneOsu::AddObject(S_COsuObject osuobj) {
	osuobj->m_uid = this->MakeUniqueObjectID();
	m_objects.push_back(osuobj);
}

void CSceneOsu::RenderCircle(float x, float y, void *udata) {
	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(x, y);
	m_mvpMatrix.model->Scale(64.0f, 64.0f, 1.0f); // TODO: Un-hardcode

	GFX->Begin(m_mvpMatrix, m_circleBuffer);
	GFX->SetTexture(m_circleTex);
	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
	GFX->End();
}

void CSceneOsu::RenderSpinner(float x, float y, void *udata) {

}

void CSceneOsu::RenderSlider(float x, float y, void *udata) {

}


///
/// COsuObject
///
COsuObject::COsuObject(float x, float y) {
	this->x = x;
	this->y = y;
}

unsigned int COsuObject::GetUID() {
	return m_uid;
}

///
/// COsuCircle
///

COsuCircle::COsuCircle(float x, float y)
	: COsuObject(x, y)
{
}

OsuObjectType COsuCircle::GetType() {
	return OsuObjectType::CIRCLE;
}