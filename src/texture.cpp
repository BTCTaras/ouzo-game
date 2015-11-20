#include "texture.h"

#include <GL/glew.h>
#include <FreeImage.h>

#include <stdio.h>

CTexture::CTexture() {
  glGenTextures(1, &m_id);
}

CTexture::CTexture(const CTexture &tex) {
  m_id = tex.m_id;
}

CTexture::~CTexture() {
  glDeleteTextures(1, &m_id);
}

CTexture::CTexture(const char *file)
  : CTexture()
{
  this->LoadFromFile(file);
}

void CTexture::LoadFromFile(const char *file) {
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
               0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image));

  FreeImage_Unload(image);
  FreeImage_Unload(bitmap);
}

unsigned int CTexture::GetWidth() {
  return m_width;
}

unsigned int CTexture::GetHeight() {
  return m_height;
}

void CTexture::Use() {
  glBindTexture(GL_TEXTURE_2D, m_id);
}
