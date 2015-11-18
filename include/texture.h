#pragma once

#include <memory>

class CTexture {
public:
  CTexture();
  CTexture(const char *file);
  ~CTexture();

  void LoadFromFile(const char *file);

  void Use();

private:
  unsigned int m_id;
};

typedef std::shared_ptr<CTexture> S_CTexture;
