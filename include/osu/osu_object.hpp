#pragma once

#include <memory>

enum OsuObjectType {
	CIRCLE,
	SLIDER,
	SPINNER
};

class COsuObject {
public:
	COsuObject();

	virtual OsuObjectType GetType() = 0;
	virtual void OnClick(int x, int y) = 0;

	void SetPosition(float x, float y);

	float x, y;
};

class COsuCircle : public COsuObject {
public:
	virtual OsuObjectType GetType() override;
	virtual void OnClick(int x, int y) override;
};

typedef std::shared_ptr<COsuObject> S_COsuObject;