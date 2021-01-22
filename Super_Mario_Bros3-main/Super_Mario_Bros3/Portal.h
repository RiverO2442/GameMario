#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	float x, y;
	int width;
	int height;
	CPortal(float x, float y);
	virtual void Render();
	//virtual void Update();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};