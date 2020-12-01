

#pragma once
#include "GameObject.h"

#define RECT_BBOX_WIDTH  16
#define RECT_BBOX_HEIGHT 16

class PIPE : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};