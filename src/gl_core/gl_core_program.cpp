#include "graphics_common/shader.hpp"

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

unsigned int CGLProgram::GetOpenGLHandle() {
	return m_id;
}

unsigned int CGLProgram::GetUniformLocation(const char *name, bool ignoreUniformNotFound) {
	std::string sname = name;

	if (m_uniformCache.find(sname) != m_uniformCache.end()) {
		return m_uniformCache[sname];
	}
	else {
		GLint id = glGetUniformLocation(m_id, name);
		if (id == -1) {
			if (ignoreUniformNotFound) {
				return -1;
			}
			else {
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

void CGLProgram::SetUniform(ShaderUniformType type, const char *name, void *values) {
	this->Use();

	GLint loc = this->GetUniformLocation(name);

	const GLfloat *floats = (const GLfloat*)values;
	const GLint *ints = (const GLint*)values;

	switch (type) {
	case ShaderUniformType::GFX_FLOAT:
		glUniform1f(loc, floats[0]);
		break;
	case ShaderUniformType::GFX_INT:
		glUniform1i(loc, ints[0]);
		break;
	case ShaderUniformType::GFX_VEC4F:
		glUniform4f(loc, floats[0], floats[1], floats[2], floats[3]);
		break;
	case ShaderUniformType::GFX_VEC4I:
		glUniform4i(loc, ints[0], ints[1], ints[2], ints[3]);
		break;
	case ShaderUniformType::GFX_VEC3F:
		glUniform3f(loc, floats[0], floats[1], floats[2]);
		break;
	case ShaderUniformType::GFX_VEC3I:
		glUniform3i(loc, ints[0], ints[1], ints[2]);
		break;
	case ShaderUniformType::GFX_VEC2F:
		glUniform2f(loc, floats[0], floats[1]);
		break;
	case ShaderUniformType::GFX_VEC2I:
		glUniform2i(loc, ints[0], ints[1]);
		break;
	case ShaderUniformType::GFX_MAT4x4F:
		glUniformMatrix4fv(loc, 1, GL_FALSE, floats);
		break;
	case ShaderUniformType::GFX_MAT3x3F:
		glUniformMatrix3fv(loc, 1, GL_FALSE, floats);
		break;
	}
}
