#pragma once
#include "GameObject.h"
#include "define.h"



class CNoCollisionObject : public CGameObject
{
	bool isRender;
public:
	CNoCollisionObject(int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};