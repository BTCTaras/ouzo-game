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

CText::CText(CFont *font, unsigned int size, const std::u32string &text)
	: CText()
{
	this->InitText(font, size, text);
}

void CText::InitText(CFont *font, unsigned int size, const std::u32string &text) {
	if (s_fontProgram == nullptr) {
		S_CShader fontShaders[] = {
		  GFX->CreateShader(ShaderType::VERTEX_SHADER, "assets/shaders/uitext"),
		  GFX->CreateShader(ShaderType::FRAGMENT_SHADER, "assets/shaders/uitext"),
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

void CText::SetText(const std::u32string &text) {
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
		vertices.push_back({ x2,      h + y2,      0.0f, u1, v1 });
		vertices.push_back({ x2,      y2,           0.0f, u1, v2 });
		vertices.push_back({ x2 + w,  h + y2,      0.0f, u2, v1 });

		// Triangle 2
		vertices.push_back({ x2,      y2,           0.0f, u1, v2 });
		vertices.push_back({ x2 + w,  y2,           0.0f, u2, v2 });
		vertices.push_back({ x2 + w,  h + y2,      0.0f, u2, v1 });

		x += gdata->advanceX >> 6;
		y += gdata->advanceY >> 6;
	}

	m_fontBufferSize = vertices.size() * sizeof(vertex_t);
	m_fontBuffer->Orphan(m_fontBufferSize, &vertices[0]);
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

	GFX->Begin(mvp, m_fontBuffer, s_fontProgram);

	S_CGLProgram fontProg = std::static_pointer_cast<CGLProgram>(s_fontProgram);
	glUniform3f(fontProg->GetUniformLocation("u_FontColour"), m_colour.r, m_colour.g, m_colour.b);

	GFX->SetTexture(atlas, 0);
	GFX->Draw(PrimitiveType::TRIANGLES);

	GFX->End();
}

S_CBuffer CText::GetBuffer() {
	return m_fontBuffer;
}
