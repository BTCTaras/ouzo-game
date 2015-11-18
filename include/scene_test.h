#pragma once

#include "scene.h"

class CSceneTest : public CScene {
public:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnResize(int newWidth, int newHeight);
};
