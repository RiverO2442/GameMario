#include "Goomba.h"
#include <algorithm>
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}
CGoomba::CGoomba(int ctype)
{
	type = ctype;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;

	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	// Simple fall down
	vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_2)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				vx = -vx;
				goomba->vx = -goomba->vx;
			}
			else //Colli with any thing else then Goomba will change direction
				if (nx != 0 && ny == 0)
				{
					vx = -vx;
				}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		if (x < 0) vx = -vx;
	}
}

void CGoomba::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGoomba::Render()
{
	int ani = -1;
	switch (type)
	{
	case GOOMBA_NORMAL:
		ani = GOOMBA_NORMAL_ANI_WALKING;
		if (state == GOOMBA_STATE_DISAPPEAR)
			return;
		else if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_NORMAL_ANI_DIE;
			state = GOOMBA_STATE_DISAPPEAR;
		}
		else if (state == GOOMBA_STATE_DIE_2) {
			ani = GOOMBA_NORMAL_ANI_WALKING;
			//state = GOOMBA_STATE_DISAPPEAR;
		}
		else if (state == GOOMBA_STATE_DIE_BY_KICK) {
			ani = GOOMBA_NORMAL_ANI_WALKING;
		}
		break;
	case GOOMBA_RED_FLY:
		ani = GOOMBA_RED_FLY_ANI_WALKING;
		if (state == GOOMBA_STATE_DISAPPEAR)
			return;
		else if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_RED_FLY_ANI_DIE;
			state = GOOMBA_STATE_DISAPPEAR;
		}
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
	}
}
