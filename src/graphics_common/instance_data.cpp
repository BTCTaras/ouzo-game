#include "graphics_common/instance_data.hpp"

#include "graphics_common/buffer.hpp"
#include "graphics_common/graphics.hpp"
#include "game.hpp"

#include <map>
#include <vector>

CInstanceData::CInstanceData() {

}

instance_id_t CInstanceData::AddInstance(S_CMatrix trans) {
    instance_id_t id = this->MakeUniqueInstanceID();
    m_instances[id] = trans;
	return id;
}

S_CBuffer CInstanceData::CreateBuffer() {
    S_CBuffer buf = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);

    std::vector<float> bufData;

    for (std::pair<instance_id_t, S_CMatrix> p : m_instances) {
        instance_id_t id = p.first;
        S_CMatrix mat = p.second;

        float *data = mat->ValuePointer();

        for (unsigned int i = 0; i < 16; ++i) {
            bufData.push_back(data[i]);
        }
    }

    buf->Orphan(bufData.size() * sizeof(float), sizeof(float), &bufData[0]);
	return buf;
}

instance_id_t CInstanceData::MakeUniqueInstanceID() {
    instance_id_t id = 0;

    while (this->GetTransformByID(id) != nullptr) {
        id++;
    }

    return id;
}

S_CMatrix CInstanceData::GetTransformByID(instance_id_t id) {
    if (m_instances.find(id) == m_instances.end()) {
        return nullptr;
    }

    return m_instances[id];
}
