#include "RECT.h"
void CRECT::Render()
{
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CRECT::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + RECT_BBOX_WIDTH;
	b = y;
}