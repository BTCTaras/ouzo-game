#pragma once

typedef struct {
  float x, y, z;
  float u, v;
} vertex_t;

class CGraphics {
public:
  ~CGraphics();
  void Init();

  void Begin();
  void End();

  static const unsigned int VERT_ATTRIB_POS;
  static const unsigned int VERT_ATTRIB_TEX_COORDS;

private:
  unsigned int m_vao;
};
