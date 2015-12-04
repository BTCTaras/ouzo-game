#include "text.hpp"

#include <GL/glew.h>

#include "shader.hpp"
#include "graphics.hpp"
#include "game.hpp"
#include "font.hpp"
#include "texture.hpp"

#include <vector>

S_CProgram CText::s_fontProgram;

CText::CText()
	: m_fontBuffer(nullptr)
{
	m_colour = { 1.0f, 1.0f, 1.0f }; // Start white
}

CText::CText(CFont *font, unsigned int size, const std::string &text)
	: CText()
{
	this->InitText(font, size, text);
}

void CText::InitText(CFont *font, unsigned int size, const std::string &text) {
	if (s_fontProgram == nullptr) {
		S_CShader fontShaders[] = {
		  GFX->CreateShader(ShaderType::GFX_VERTEX_SHADER, "assets/shaders/uitext"),
		  GFX->CreateShader(ShaderType::GFX_FRAGMENT_SHADER, "assets/shaders/uitext"),
		};

		s_fontProgram = GFX->CreateProgram(2, fontShaders);
	}

	if (m_fontBuffer == nullptr) {
		m_fontBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	}

	m_size = size;
	m_font = font;

	this->SetText(text);
}

void CText::SetText(const std::string &text) {
	m_text = text;
	this->CreateTextBuffer();
}

CText::~CText() {}

void CText::CreateTextBuffer() {
	std::vector<vertex_t> vertices;

	float x = 0;
	float y = 0;

	for (size_t i = 0; i < m_text.length(); ++i) {
		unsigned long c = m_text[i];
		if (c == (unsigned long) ' ') {
			x += m_size / 4; // TODO: Un-hardcode this.
		}

		glyph_data_t *gdata = m_font->GetGlyphData(c);

		float x2 = x + gdata->bitmapLeft;
		float y2 = -y - gdata->bitmapTop;
		float w = (float)gdata->width;
		float h = (float)gdata->height;

		float u1 = gdata->u1;
		float v1 = gdata->v2 - gdata->v1;
		float u2 = gdata->u2;
		float v2 = v1 - gdata->v2;

		// Triangle 1
		vertices.push_back({ x2,      h + y2,      0.0f, 1.0f, u1, v1, 0.0f, 0.0f, 1.0f });
		vertices.push_back({ x2,      y2,          0.0f, 1.0f, u1, v2, 0.0f, 0.0f, 1.0f });
		vertices.push_back({ x2 + w,  h + y2,      0.0f, 1.0f, u2, v1, 0.0f, 0.0f, 1.0f });

		// Triangle 2
		vertices.push_back({ x2,      y2,          0.0f, 1.0f, u1, v2, 0.0f, 0.0f, 1.0f });
		vertices.push_back({ x2 + w,  y2,          0.0f, 1.0f, u2, v2, 0.0f, 0.0f, 1.0f });
		vertices.push_back({ x2 + w,  h + y2,      0.0f, 1.0f, u2, v1, 0.0f, 0.0f, 1.0f });

		x += gdata->advanceX >> 6;
		y += gdata->advanceY >> 6;
	}

	m_fontBufferSize = vertices.size() * sizeof(vertex_t);
	m_fontBuffer->Orphan(m_fontBufferSize, sizeof(vertex_t), &vertices[0]);
}

void CText::SetColour(colour_t colour) {
	m_colour = colour;
}

void CText::Render(mvp_matrix_t &mvp) {
	S_CTexture atlas = m_font->GetAtlas(m_size);

	if (atlas == nullptr) {
		atlas = m_font->CreateAtlas(m_size);
		this->CreateTextBuffer();
	}

	s_fontProgram->SetUniform(ShaderUniformType::GFX_VEC3F, "u_FontColour", &m_colour);

	static S_CDrawAttribs attribs = GFX->CreateDrawAttribs(m_fontBuffer);

	GFX->SetDrawProgram(s_fontProgram);
	GFX->SetDrawTransform(mvp);
	GFX->SetDrawBuffer(m_fontBuffer);
	GFX->SetDrawAttributes(attribs);
	GFX->SetDrawTexture(atlas);

	GFX->Draw(PrimitiveType::GFX_TRIANGLE_STRIP);
}

S_CBuffer CText::GetBuffer() {
	return m_fontBuffer;
}
