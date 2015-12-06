#include "graphics_common/texture.hpp"

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

void CGLTexture::LoadFromFile(const char *file, FilterType filterType) {
	FIBITMAP *bitmap = FreeImage_Load(FreeImage_GetFileType(file, 0), file);

	if (!bitmap) {
		fprintf(stderr, "Failed to load image \"%s\"!!\n", file);
		return;
	}

	FreeImage_FlipVertical(bitmap);

	m_width = FreeImage_GetWidth(bitmap);
	m_height = FreeImage_GetHeight(bitmap);

	unsigned int bpp = FreeImage_GetBPP(bitmap);

	if (bpp != 32) {
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
	}

	glBindTexture(GL_TEXTURE_2D, m_id);

	GLenum gl_filterType;

	switch (filterType) {
	case FilterType::LINEAR:
		gl_filterType = GL_LINEAR;
		break;

	case FilterType::NEAREST_NEIGHBOUR:
		gl_filterType = GL_NEAREST;
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filterType);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));

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