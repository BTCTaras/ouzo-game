#ifdef _WIN32
#include "d3d/d3d_graphics.hpp"
#include "d3d/winutils.hpp"
#include "shader.hpp"
#include "game.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <string.h>

#include <d3d11.h>
#include <d3dcompiler.h>

CD3DShader::CD3DShader(ShaderType type)
	:	CShader(type),
		m_shader(NULL)
{
}

CD3DShader::~CD3DShader() {
	if (m_shader != NULL) {
		m_shader->Release();
	}
}

void CD3DShader::LoadFromFile(const char *file) {
	std::string typeExt;

	switch (m_type) {
	case ShaderType::GFX_VERTEX_SHADER:
		typeExt = "vert";
		break;

	case ShaderType::GFX_FRAGMENT_SHADER:
		typeExt = "frag";
		break;
	}

	std::ostringstream filePath;
	filePath << file << "." << typeExt << ".hlsl";

	std::ifstream in(filePath.str());
	std::string line;
	std::string source;

	while (std::getline(in, line)) {
		source += line + "\n";
	}

	this->Load(filePath.str().c_str(), source.c_str());
}

void CD3DShader::Load(const char *chunkName, const char *source) {
	m_chunkName = chunkName;

	char *entryPoint, *shaderProfile;
	switch (m_type) {
	case ShaderType::GFX_VERTEX_SHADER:
		entryPoint = "VShader";
		shaderProfile = "vs_3_0";
		break;
	case ShaderType::GFX_FRAGMENT_SHADER:
		entryPoint = "FShader";
		shaderProfile = "ps_3_0";
		break;
	}

	ID3DBlob *compiledShader;

	HRESULT result;

	result = D3DCompile(
		source,
		strlen(source),
		chunkName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderProfile,
		0, 0,
		&compiledShader,
		NULL
	);

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to compile shader \"%s\"!!\n", chunkName);
		return;
	}

	switch (m_type) {
	case ShaderType::GFX_VERTEX_SHADER:
		result = GFX_D3D_DEV->CreateVertexShader(
			compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			(ID3D11VertexShader**)&m_shader
		);
		break;

	case ShaderType::GFX_FRAGMENT_SHADER:
		result = GFX_D3D_DEV->CreatePixelShader(
			compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			(ID3D11PixelShader**)&m_shader
		);
		break;
	}

	if (!WinUtils::CheckResult(result)) {
		fprintf(stderr, "Failed to create shader \"%s\"!!\n", chunkName);
		return;
	}
}

const std::string& CD3DShader::GetChunkName() {
	return m_chunkName;
}

void CD3DShader::OnAttach() {}

void CD3DShader::OnDetach() {}

#endif