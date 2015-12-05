#include "graphics_common/shader.hpp"

CShader::CShader(ShaderType type)
	: m_type(type)
{
}

ShaderType CShader::GetType() {
	return m_type;
}