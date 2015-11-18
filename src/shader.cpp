#include "shader.h"

#include <GL/glew.h>
#include <fstream>
#include <stdio.h>
#include <memory>

CShader::CShader(unsigned int type)
  : m_attachmentCounter(0),
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

    GLboolean success, buflen;
    glGetshaderiv(m_id, GL_COMPILE_STATUS, &success);
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

unsigned int CShader::GetHandle() {
  return m_id;
}

const std::string& CShader::GetChunkName() {
  return m_chunkName;
}
