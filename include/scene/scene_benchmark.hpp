#pragma once

#include "scene.hpp"

#include "graphics_common/buffer.hpp"
#include "graphics_common/texture.hpp"
#include "graphics_common/matrix.hpp"
#include "graphics_common/draw_attribs.hpp"
#include "graphics_common/shader.hpp"
#include "graphics_common/text.hpp"

class CSceneBenchmark : public CScene {
public:
	CSceneBenchmark();
	
	virtual void OnInit() override;
	virtual void OnResize(int width, int height) override;

	virtual void OnRender() override;

private:
	S_CBuffer m_buffer;
	S_CTexture m_texture;

	S_CBuffer m_starBuffer;
	S_CDrawAttribs m_starAttribs;
	mvp_matrix_t m_mvpMatrix;

	S_CProgram m_starProgram;

	size_t m_starCount;

	CText m_dtText;
	CFont m_font;
};