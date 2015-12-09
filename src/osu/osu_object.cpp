#include "osu/osu_object.hpp"

COsuObject::COsuObject()
	: x(0), y(0)
{
}

void COsuObject::SetPosition(float x, float y) {
	this->x = x;
	this->y = y;
}
