#pragma once

#include "texture.hpp"

#include <memory>

class CAtlasFactory {
public:
	CAtlasFactory(unsigned int width, unsigned int height, unsigned int channels);

	virtual void Write(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *data) = 0;
	virtual S_CTexture GetTexture(FilterType type = FilterType::LINEAR) = 0;

	unsigned int GetWidth();
	unsigned int GetHeight();

protected:
	unsigned int m_width, m_height;
	unsigned int m_channels;
};

typedef std::shared_ptr<CAtlasFactory> S_CAtlasFactory;

class CGLAtlasFactory : public CAtlasFactory {
public:
	CGLAtlasFactory(unsigned int width, unsigned int height, unsigned int channels);
	~CGLAtlasFactory();

	virtual void Write(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *data) override;
	virtual S_CTexture GetTexture(FilterType type = FilterType::LINEAR) override;

private:
	unsigned int GetImageFormat();

	S_CGLTexture m_atlasTex;
};

typedef std::shared_ptr<CGLAtlasFactory> S_CGLAtlasFactory;
