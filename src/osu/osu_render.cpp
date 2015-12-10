#include "osu/osu_render.hpp"
#include "osu/osu_object.hpp"

#include "graphics_common/buffer.hpp"
#include "graphics_common/graphics.hpp"
#include "graphics_common/draw_attribs.hpp"
#include "graphics_common/sprite_verts.hpp"

#include "game.hpp"

#include <algorithm>

const float STAR_SIZE = 64.0f;

///
///	CCircleRenderer
///

void CCircleRenderer::Init() {
	m_buffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	m_buffer->Orphan(SPRITE_VERTS_SIZE, sizeof(vertex_t), (void*)SPRITE_VERTS);

	m_drawAttribs = GFX->CreateDrawAttribs(m_buffer);
}

void CCircleRenderer::DoRender(mvp_matrix_t &mvp) {
	if (m_rebuildNeeded) {
		this->RebuildInstances();
		m_rebuildNeeded = false;
	}

	GFX->SetDrawProgram(GFX->GetDefaultProgram());
	GFX->SetDrawTransform(mvp);
	GFX->SetDrawBuffer(m_buffer);
	GFX->SetDrawAttributes(m_drawAttribs);
	GFX->SetDrawTexture(m_circleTex);
	
	GFX->DrawInstanced(PrimitiveType::GFX_TRIANGLE_STRIP, m_elements.size());
}

void CCircleRenderer::SetCircleTexture(S_CTexture tex) {
	m_circleTex = tex;
}

void CCircleRenderer::RebuildInstances() {
	CInstanceData data;

	for (element_t elem : m_elements) {
		S_CMatrix mat = GFX->CreateIdentityMatrix();
		mat->Translate(elem.x, elem.y, 0.0f);
		mat->Scale(STAR_SIZE, STAR_SIZE, 1.0f);
		data.AddInstance(mat);
	}

	data.LoadInto(m_drawAttribs);
}

///
/// COsuRenderer
///

COsuRenderer::COsuRenderer()
	: m_rebuildNeeded(true)
{
}

void COsuRenderer::AddElement(float x, float y) {
	m_elements.push_back({ x, y });
	m_rebuildNeeded = true;
}

void COsuRenderer::RemoveElement(float x, float y) {
	std::remove_if(m_elements.begin(), m_elements.end(), [=](element_t elem) {
		return elem.x == x && elem.y == y;
	});
}

void COsuRenderer::RebuildInstances() {}
