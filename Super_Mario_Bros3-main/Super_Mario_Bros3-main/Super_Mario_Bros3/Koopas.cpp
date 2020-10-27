#include "Koopas.h"
#include <algorithm>
#include "Goomba.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		y = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else if (state == KOOPAS_STATE_SHELLING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else if (state == KOOPAS_STATE_SPINNING)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else if(state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	vy += KOOPAS_GRAVITY * dt;
	//if (state == KOOPAS_STATE_SHELLING)
		//vy = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE)
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
		if(nx != 0 && ny != 0)
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;
		

		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
				vx = -vx;
				Koopas->vx = -Koopas->vx;
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* Goomba = dynamic_cast<CGoomba*>(e->obj);
				vx = -vx;
				Goomba->vx = -Goomba->vx;
			}
			else //Colli with any thing else then Koopas will change direction
				if (nx != 0 && ny == 0)
				{
					nx = -nx;
					vx = -vx;
				}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		if (x <= 0)
			if(vx < 0)
			vx = -vx;
	}
}

void CKoopas::CalcPotentialCollisions(
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

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	if (state == KOOPAS_STATE_SHELLING) {
		ani = KOOPAS_ANI_SHELL_DOWN;
	}
	if (state == KOOPAS_STATE_SPINNING) {
		ani = KOOPAS_ANI_SPINNING;
	}
	else if (state == KOOPAS_STATE_WALKING)
	{
		if(vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	    else ani = KOOPAS_ANI_WALKING_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SPINNING:
		if(nx < 0) vx = - KOOPAS_SPINNING_SPEED;
		if(nx > 0) vx = KOOPAS_SPINNING_SPEED;
		vy = 0;
		break;
	case KOOPAS_STATE_SHELLING:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		if (nx > 0)
		vx = KOOPAS_WALKING_SPEED;
		if (nx < 0)
		vx = -KOOPAS_WALKING_SPEED;
		if (nx == 0)
			vx = 0;
	}

}