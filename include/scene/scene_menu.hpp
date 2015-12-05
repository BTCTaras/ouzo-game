#pragma once

#include "ui/ui.hpp"
#include "graphics_common/shader.hpp"
#include "graphics_common/graphics.hpp"
#include "graphics_common/texture.hpp"
#include "graphics_common/font.hpp"
#include "graphics_common/text.hpp"
#include "graphics_common/buffer.hpp"

class CSceneMenu : public CSceneUI {
public:
	CSceneMenu();
	~CSceneMenu();

	virtual void OnUpdate() override;
	virtual void OnLeave(CScene *scene) override;
	virtual void OnRender() override;
	virtual void OnInitUI() override;

	virtual S_CTexture GetBackgroundTexture() override;

private:
	S_CTexture m_buttonTexture;
	S_CTexture m_texture;

	S_CProgram m_fontProgram;
	CFont m_font;
	CText m_helloWorld;

	S_CBuffer m_testBuffer;
};
