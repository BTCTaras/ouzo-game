#pragma once

#pragma once

#include <memory>

class CBuffer;
typedef std::shared_ptr<CBuffer> S_CBuffer;

namespace OBJ {
	S_CBuffer Load(const char *source);
	S_CBuffer LoadFromFile(const char *file);
}