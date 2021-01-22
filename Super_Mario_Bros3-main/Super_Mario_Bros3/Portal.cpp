#include "Portal.h"


CPortal::CPortal( float x, float y)
{
	this->x = x;
	this->y = y;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float x, y;
	this->GetPosition(x, y);
	l = x;
	t = y;
	r = x + PORTAL_WIDTH;
	b = y + PORTAL_HEIGHT;
}