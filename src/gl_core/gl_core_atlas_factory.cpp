#include "atlas_factory.hpp"

#include "texture.hpp"

#include <stdlib.h>

#include <GL/glew.h>

CGLAtlasFactory::CGLAtlasFactory(unsigned int width, unsigned int height, unsigned int channels)
	: CAtlasFactory(width, height, channels)
{
	m_atlasTex.reset(new CGLTexture);

	glBindTexture(GL_TEXTURE_2D, m_atlasTex->GetOpenGLHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char *emptyData = (unsigned char*)malloc(width * height * channels * sizeof(unsigned char));
	GLenum format = this->GetImageFormat();
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, emptyData);
	free(emptyData);
}

CGLAtlasFactory::~CGLAtlasFactory() {
}

void CGLAtlasFactory::Write(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *data) {
	glBindTexture(GL_TEXTURE_2D, m_atlasTex->GetOpenGLHandle());

	GLenum format = this->GetImageFormat();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, format, GL_UNSIGNED_BYTE, data);
}

S_CTexture CGLAtlasFactory::GetTexture(FilterType type) {

	GLenum glFilterType =
		(type == FilterType::NEAREST_NEIGHBOUR ? GL_NEAREST : GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, m_atlasTex->GetOpenGLHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterType);
	return m_atlasTex;
}

unsigned int CGLAtlasFactory::GetImageFormat() {
	GLenum format;

  switch (m_channels) {
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
  default:
    format = GL_RGBA;
    break;
	}

	return format;
}
