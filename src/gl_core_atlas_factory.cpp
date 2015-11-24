#include "atlas_factory.hpp"

#include "texture.hpp"

#include <string.h>

#include <GL/glew.h>

CGLAtlasFactory::CGLAtlasFactory(unsigned int width, unsigned int height, unsigned int channels)
  : CAtlasFactory(width, height, channels)
{
  m_data = (unsigned char*)malloc(width * height * channels * sizeof(unsigned char));
}

CGLAtlasFactory::~CGLAtlasFactory() {
  if (m_data != NULL) {
    free(m_data);
  }
}

void CGLAtlasFactory::Write(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *data) {
  unsigned int i = (y * m_width + x) * m_channels;
  memcpy(&m_data[i], data, w * h * m_channels);
}

S_CTexture CGLAtlasFactory::MakeTexture(FilterType type) {
  S_CGLTexture tex(new CGLTexture);
  unsigned int handle = tex->GetOpenGLHandle();

  GLenum glFilterType =
    (type == FilterType::NEAREST_NEIGHBOUR ? GL_NEAREST : GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilterType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilterType);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
  }

  glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
  return tex;
}
