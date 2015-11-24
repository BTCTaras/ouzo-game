#include "texture.hpp"

#include <GL/glew.h>
#include <FreeImage.h>

#include <stdio.h>

CGLTexture::CGLTexture() {
  glGenTextures(1, &m_id);
}

CGLTexture::~CGLTexture() {
  glDeleteTextures(1, &m_id);
}

CGLTexture::CGLTexture(const char *file)
  : CGLTexture()
{
  this->LoadFromFile(file);
}

void CGLTexture::LoadFromFile(const char *file) {
  FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType(file, 0), file);
  FIBITMAP *image = FreeImage_ConvertTo32Bits(bitmap); // Make sure there is an alpha channel
  FreeImage_FlipVertical(image); // Images are loaded upside down for some reason

  if (!bitmap) {
    fprintf(stderr, "Failed to load image \"%s\"!!\n", file);
  }

  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  m_width = FreeImage_GetWidth(image);
  m_height = FreeImage_GetHeight(image);

  if (glewIsSupported("GL_ARB_texture_compression")) {
    // Compress if we can
    // TODO: DXT3 might not be supported on all platforms.
    // -> Perform a check that figures out whether it's available.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, m_width, m_height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image));
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image));
  }

  FreeImage_Unload(image);
  FreeImage_Unload(bitmap);
}

unsigned int CGLTexture::GetOpenGLHandle() {
  return m_id;
}

unsigned int CGLTexture::GetWidth() {
  return m_width;
}

unsigned int CGLTexture::GetHeight() {
  return m_height;
}

void CGLTexture::Use() {
  glBindTexture(GL_TEXTURE_2D, m_id);
}