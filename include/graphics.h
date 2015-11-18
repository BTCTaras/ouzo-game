#pragma once

#include "shader.h"

#include <glm/glm.hpp>

///
/// Represents a vertex. Values in order (floats): x, y, z, u, v.
///
typedef struct {
  float x, y, z;
  float u, v;
} vertex_t;

typedef struct {
  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 model;
} mvp_matrix_t;

class CGraphics {
public:
  ~CGraphics();
  void Init();

  ///
  /// Enables all vertex attributes and passes them to the current program.
  ///
  void Begin(mvp_matrix_t &mvp, S_CProgram program = nullptr);

  ///
  /// Disables all vertex attributes *and unbinds any bound programs*.
  ///
  void End();

  ///
  /// The vertex attribute containing the vertex position.
  ///
  static const unsigned int VERT_ATTRIB_POS;

  ///
  /// The vertex atrtribute containing the texture coordinates.
  ///
  static const unsigned int VERT_ATTRIB_TEX_COORDS;

private:
  unsigned int m_vao; // Stores the vertex attributes.

};
