#pragma once

#include <memory>

class CTexture {
public:
  CTexture();
  CTexture(const char *file);
  CTexture(const CTexture &tex);
  ~CTexture();

  void LoadFromFile(const char *file);

  void Use();

  unsigned int GetWidth();
  unsigned int GetHeight();

private:
  unsigned int m_id;
  unsigned int m_width, m_height;
};

typedef std::shared_ptr<CTexture> S_CTexture;
