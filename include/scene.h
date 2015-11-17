#pragma once

#include <memory>

class CScene {
public:
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnInit();
	virtual void OnLeave(CScene *newScene);
};

typedef std::shared_ptr<CScene> S_CScene;