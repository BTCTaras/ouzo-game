#pragma once

#include "matrix.hpp"
#include "buffer.hpp"

#include <map>

typedef unsigned int instance_id_t;

class CInstanceData {
public:
    CInstanceData();
    instance_id_t AddInstance(S_CMatrix trans);

    S_CBuffer CreateBuffer();

private:
    instance_id_t MakeUniqueInstanceID();
    S_CMatrix GetTransformByID(instance_id_t id);

    std::map<instance_id_t, S_CMatrix> m_instances;
};
