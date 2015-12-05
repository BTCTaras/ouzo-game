#pragma once

#include <memory>

enum FilterType {
	LINEAR, NEAREST_NEIGHBOUR
};

class CTexture {
public:
	virtual void LoadFromFile(const char *file) = 0;

	virtual void Use() = 0;

	virtual unsigned int GetWidth() = 0;
	virtual unsigned int GetHeight() = 0;
};

typedef std::shared_ptr<CTexture> S_CTexture;

class CGLTexture : public CTexture {
public:
	CGLTexture();
	CGLTexture(const char *file);
	~CGLTexture();

	virtual unsigned int GetWidth() override;
	virtual unsigned int GetHeight() override;

	virtual void Use() override;

	virtual void LoadFromFile(const char *file) override;

	unsigned int GetOpenGLHandle();

private:
	unsigned int m_id;
	unsigned int m_width, m_height;
};

typedef std::shared_ptr<CGLTexture> S_CGLTexture;
