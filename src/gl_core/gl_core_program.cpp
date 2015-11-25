#include "shader.hpp"

#include <stdio.h>
#include <GL/glew.h>

CGLProgram::CGLProgram()
  : m_id(0)
{
  m_id = glCreateProgram();
}

CGLProgram::~CGLProgram() {
  glDeleteProgram(m_id);
}

CGLProgram::CGLProgram(size_t count, S_CShader *shaders)
  : CGLProgram()
{
  this->LoadFromShaders(count, shaders);
}

void CGLProgram::LoadFromShaders(size_t count, S_CShader *shaders) {
  for (size_t i = 0; i < count; ++i) {
    S_CGLShader shader = std::static_pointer_cast<CGLShader>(shaders[i]);
    glAttachShader(m_id, shader->GetOpenGLHandle());
    shader->OnAttach();
  }

  this->Link();

  for (size_t i = 0; i < count; ++i) {
    S_CGLShader shader = std::static_pointer_cast<CGLShader>(shaders[i]);
    glDetachShader(m_id, shader->GetOpenGLHandle());
    shader->OnDetach();
  }
}

void CGLProgram::Use() {
  if (m_id == 0) {
    fprintf(stderr, "Warning: Tried to use uninitialised/invalid program!!\n");
    return;
  }

  glUseProgram(m_id);
}

unsigned int CGLProgram::GetUniformLocation(const char *name, bool ignoreUniformNotFound) {
  std::string sname = name;

  if (m_uniformCache.find(sname) != m_uniformCache.end()) {
    return m_uniformCache[sname];
  } else {
    GLint id = glGetUniformLocation(m_id, name);
    if (id == -1) {
      if (ignoreUniformNotFound) {
        return -1;
      } else {
        fprintf(stderr, "CGLProgram::GetUniformLocation: Program has no uniform called \"%s\"!!\n", name);
        return -1;
      }
    }

    m_uniformCache[sname] = id;
    return id;
  }
}

void CGLProgram::Link() {
  glLinkProgram(m_id);

  GLint success, buflen;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &buflen);

  if (!success) {
    std::unique_ptr<GLchar> str(new GLchar[buflen]);
    glGetProgramInfoLog(m_id, buflen, NULL, str.get());
    fprintf(stderr, "Failed to link program: %s\n", str.get());
  }
}
