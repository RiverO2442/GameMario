#pragma once
#include "GameObject.h"
#include "define.h"


class CMenuGame : public CGameObject
{
	bool isRender = false;
public:
	CMenuGame();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
