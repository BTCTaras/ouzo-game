#include "scene_gamemode.hpp"

#include "graphics_common/graphics.hpp"
#include "graphics_common/texture.hpp"
#include "graphics_common/buffer.hpp"

#include "osu/osu_beatmap.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <map>

class CSceneOsu : public CSceneGamemode {
public:
	virtual void OnInit() override;

	virtual unsigned short GetGamemodeID() override;
	virtual void OnRender() override;
	virtual void OnResize(int width, int height) override;

	void SetBackgroundTex(S_CTexture tex);

private:
	S_CBuffer m_backgroundBuffer;
	S_CTexture m_backgroundTex;
	mvp_matrix_t m_mvpMatrix;

	COsuBeatmap m_beatmap;
};
