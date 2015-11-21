#include "shader.h"

#include <GL/glew.h>
#include <fstream>
#include <stdio.h>
#include <memory>

CShader::CShader(unsigned int type)
: m_id(0),
m_attachmentCounter(0),
m_chunkName("N/A")
{
  m_id = glCreateShader(type);
}

CShader::~CShader() {
  if (m_attachmentCounter > 0) {
    fprintf(stderr, "Warning: Can't delete shader \"%s\" because it is still attached to a program.", m_chunkName.c_str());
  }

  glDeleteShader(m_id);
}

CShader::CShader(unsigned int type, const char *file)
: CShader(type)
{
  this->LoadFromFile(file);
}

void CShader::LoadFromFile(const char *file) {
  std::ifstream in(file);
  std::string line;
  std::string source;

  while (std::getline(in, line)) {
    source += line + "\n";
  }

  this->Load(file, source.c_str());
}

void CShader::Load(const char *chunkName, const char *source) {
  m_chunkName = chunkName;
  glShaderSource(m_id, 1, &source, NULL);
  this->Compile();
}

void CShader::Compile() {
  glCompileShader(m_id);

  GLint success, buflen;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &buflen);

  if (!success) {
    std::unique_ptr<GLchar> str(new GLchar[buflen]);
    glGetShaderInfoLog(m_id, buflen, NULL, str.get());
    fprintf(stderr, "Failed to compile shader \"%s\": %s\n", m_chunkName.c_str(), str.get());
  }
}

void CShader::OnAttach() {
  m_attachmentCounter++;
}

void CShader::OnDetach() {
  m_attachmentCounter--;
}

unsigned int CShader::GetHandle() {
  return m_id;
}

const std::string& CShader::GetChunkName() {
  return m_chunkName;
}

CProgram::CProgram()
  : m_id(0)
{
  m_id = glCreateProgram();
}

CProgram::~CProgram() {
  glDeleteProgram(m_id);
}

CProgram::CProgram(size_t count, S_CShader *shaders)
  : CProgram()
{
  this->LoadFromShaders(count, shaders);
}

void CProgram::LoadFromShaders(size_t count, S_CShader *shaders) {
  for (size_t i = 0; i < count; ++i) {
    S_CShader shader = shaders[i];
    glAttachShader(m_id, shader->m_id);
    shader->OnAttach();
  }

  this->Link();

  for (size_t i = 0; i < count; ++i) {
    S_CShader shader = shaders[i];
    glDetachShader(m_id, shader->m_id);
    shader->OnDetach();
  }
}

void CProgram::Use() {
  if (m_id == 0) {
    fprintf(stderr, "Warning: Tried to use uninitialised/invalid program!!\n");
    return;
  }

  glUseProgram(m_id);
}

int CProgram::GetUniformLocation(const char *name, bool ignoreUniformNotFound) {
  std::string sname = name;

  if (m_uniformCache.find(sname) != m_uniformCache.end()) {
    return m_uniformCache[sname];
  } else {
    GLint id = glGetUniformLocation(m_id, name);
    if (id == -1) {
      if (ignoreUniformNotFound) {
        return -1;
      } else {
        fprintf(stderr, "CProgram::GetUniformLocation: Program has no uniform called \"%s\"!!\n", name);
        return -1;
      }
    }

    m_uniformCache[sname] = id;
    return id;
  }
}

void CProgram::Link() {
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
