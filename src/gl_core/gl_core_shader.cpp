#include "shader.hpp"

#include <GL/glew.h>
#include <fstream>
#include <stdio.h>
#include <memory>

CGLShader::CGLShader(unsigned int type)
	: m_id(0),
	m_attachmentCounter(0),
	m_chunkName("N/A")
{
	m_id = glCreateShader(type);
}

CGLShader::~CGLShader() {
	if (m_attachmentCounter > 0) {
		fprintf(stderr, "Warning: Can't delete shader \"%s\" because it is still attached to a program.", m_chunkName.c_str());
	}

	glDeleteShader(m_id);
}

CGLShader::CGLShader(unsigned int type, const char *file)
	: CGLShader(type)
{
	this->LoadFromFile(file);
}

void CGLShader::LoadFromFile(const char *file) {
	std::ifstream in(file);
	std::string line;
	std::string source;

	while (std::getline(in, line)) {
		source += line + "\n";
	}

	this->Load(file, source.c_str());
}

void CGLShader::Load(const char *chunkName, const char *source) {
	m_chunkName = chunkName;
	glShaderSource(m_id, 1, &source, NULL);
	this->Compile();
}

void CGLShader::Compile() {
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

void CGLShader::OnAttach() {
	m_attachmentCounter++;
}

void CGLShader::OnDetach() {
	m_attachmentCounter--;
}

unsigned int CGLShader::GetOpenGLHandle() {
	return m_id;
}

const std::string& CGLShader::GetChunkName() {
	return m_chunkName;
}
