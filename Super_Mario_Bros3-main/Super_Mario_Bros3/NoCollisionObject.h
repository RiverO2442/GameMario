#pragma once
#include "GameObject.h"

#define NoCollisionObjects_BBOX_WIDTH  16
#define NoCollisionObjects_BBOX_HEIGHT 16

class CNoCollisionObject : public CGameObject
{
	bool isRender;
public:
	CNoCollisionObject(int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
