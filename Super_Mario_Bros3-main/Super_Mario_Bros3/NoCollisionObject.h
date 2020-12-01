#pragma once
#include "GameObject.h"

#define OBJECT_BBOX_WIDTH  16
#define OBJECT_BBOX_HEIGHT 16

class CNoCollitionObject : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};