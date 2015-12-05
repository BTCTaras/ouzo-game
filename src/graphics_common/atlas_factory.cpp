#include "graphics_common/atlas_factory.hpp"

CAtlasFactory::CAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) {
	m_width = width;
	m_height = height;
	m_channels = channels;
}

unsigned int CAtlasFactory::GetWidth() {
	return m_width;
}

unsigned int CAtlasFactory::GetHeight() {
	return m_height;
}
