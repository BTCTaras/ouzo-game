#include "graphics_common/matrix.hpp"

S_CMatrix operator*(S_CMatrix first, S_CMatrix second) {
	return *first * second;
}
