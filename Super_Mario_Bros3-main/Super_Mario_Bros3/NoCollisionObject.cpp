#include "NoCollisionObject.h"

void CNoCollitionObject::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CNoCollitionObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//l = x;
	//t = y;
	//r = x + OBJECT_BBOX_WIDTH;
	//b = y + OBJECT_BBOX_HEIGHT;
}