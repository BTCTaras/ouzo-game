#include "graphics_common/instance_data.hpp"
#include "graphics_common/graphics.hpp"
#include "game.hpp"

CInstanceData::CInstanceData()
	: m_hasChanged(false)
{
}

instance_id_t CInstanceData::AddInstance(S_CMatrix matrix) {
	instance_id_t id = (instance_id_t)m_instances.size();
	m_instances.push_back(matrix);
	m_hasChanged = true;
	return id;
}

S_CMatrix CInstanceData::GetInstance(instance_id_t id, bool willModify) {
	if (willModify) {
		m_hasChanged = true;
	}

	return m_instances[id];
}

void CInstanceData::LoadInto(S_CDrawAttribs attribs) {
	if (m_hasChanged) { // prevent unnecessary buffer recreation
		this->CreateBuffers();
	}

	for (int i = 0; i < 4; ++i) {
		attribs->SetSource((AttribType)(AttribType::OFFSET1 + i), m_buffers[i], 0);
	}
}
	
void CInstanceData::CreateBuffers() {
	std::vector<float> buffers[4];

	int i = 0;
	for (S_CMatrix mat : m_instances) {
		float *ptr = mat->ValuePointer();

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				buffers[i].push_back(ptr[i * 4 + j]);
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		m_buffers[i] = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);

		size_t bufSize = sizeof(float) * buffers[i].size();
		m_buffers[i]->Orphan(bufSize, sizeof(float) * 4, &buffers[i][0]);
	}
	
	m_hasChanged = false;
}
