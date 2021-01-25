#include "PIPE.h"

PIPE::PIPE(int ctype)
{
	type = ctype;
}


void PIPE::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void PIPE::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == PIPE_TYPE_NORMAL)
	{
		l = x + PIPE_BBOX_FIX;
		t = y;
		r = x + PIPE_BBOX_WIDTH_NORMAL - PIPE_BBOX_FIX;
		b = y + PIPE_BBOX_HEIGHT;
	}
	else
	{
		l = x + PIPE_BBOX_FIX;
		t = y;
		r = x + PIPE_BBOX_WIDTH - PIPE_BBOX_FIX;
		b = y + PIPE_BBOX_HEIGHT;
	}
}