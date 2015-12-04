#include "scene_gamemode.hpp"

#include "graphics.hpp"
#include "texture.hpp"
#include "buffer.hpp"

#include <vector>
#include <memory>
#include <functional>
#include <map>

// Must be sorted by render complexity!!
enum OsuObjectType {
	CIRCLE, SPINNER, SLIDER
};

class COsuObject {
	friend class CSceneOsu;

public:
	COsuObject(float x = 0.0f, float y = 0.0f);
	unsigned int GetUID();

	virtual OsuObjectType GetType() = 0;

	float x, y;

private:
	unsigned int m_uid;

};

typedef std::shared_ptr<COsuObject> S_COsuObject;

class COsuCircle : public COsuObject {
public:
	COsuCircle(float x = 0.0f, float y = 0.0f);

	virtual OsuObjectType GetType() override;
};

typedef std::shared_ptr<COsuCircle> S_COsuCircle;

class CSceneOsu : public CSceneGamemode {
public:
	CSceneOsu();

	virtual unsigned short GetGamemodeID() override;
	virtual void OnRender() override;
	virtual void OnResize(int width, int height) override;

	void AddObject(S_COsuObject obj);

	void SetBackgroundTex(S_CTexture tex);

private:
	void RenderCircle(float x, float y, void *udata);
	void RenderSpinner(float x, float y, void *udata);
	void RenderSlider(float x, float y, void *udata);

	unsigned int MakeUniqueObjectID();

	std::vector<S_COsuObject> m_objects;
	unsigned int m_nextUID;
	S_CBuffer m_circleBuffer;
	S_CDrawAttribs m_circleAttribs;
	S_CTexture m_circleTex, m_backgroundTex;
	mvp_matrix_t m_mvpMatrix;
};
