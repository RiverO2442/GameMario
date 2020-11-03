#include "FIREBALL.h"
#include <algorithm>
#include "Goomba.h"
#include "PlayScene.h"

FIREBALL::FIREBALL()
{
	SetState(FIREBALL_STATE_FLYING);
	nx = 0;
}
void FIREBALL::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;

	if (state == FIREBALL_STATE_DIE)
		y = y + FIREBALL_BBOX_HEIGHT;
	else bottom = y + FIREBALL_BBOX_HEIGHT;
}

void FIREBALL::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	//if (state == KOOPAS_STATE_SHELLING)
		//vy = 0;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	// turn off collision when die 
	if (state != FIREBALL_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (y <= upBoudary)
		if (vy < 0)
			vy = -vy;
	if (isUsed == false)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetisFiring() == true)
		{
			if(isUsed == false)
			if (mario->GetisAlreadyFired() == false)
			{
				x = mario->x;
				y = mario->y;
				SetSpeed(mario->nx*0.1f, 0.1f);
				mario->SetisAlreadyFired(true);
				upBoudary = mario->y;
			}
		}
	}
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
		//x += min_tx * dx + nx * 0.4f;

		//if(nx != 0 && ny != 0)
		//y += min_ty * dy + ny * 0.4f;

		//if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			//LPCOLLISIONEVENT e = coEventsResult[i];
			//if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			//{
			//	CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
			//	vx = -vx;
			//	Koopas->vx = -Koopas->vx;
			//}
			//else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			//{
			//	CGoomba* Goomba = dynamic_cast<CGoomba*>(e->obj);
			//	vx = -vx;
			//	Goomba->vx = -Goomba->vx;
			//}
			//else //Colli with any thing else then Koopas will change direction
			if (ny != 0 )
			{
				if (ny > 0)
					upBoudary = y;
					vy = -vy;
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		/*if (ny <= )
		{
			vy = -0.7*vy;
		}*/
		if (x <= 0)
			if (vx < 0)
				vx = -vx;
	}
}

void FIREBALL::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CMario*>(e->obj))
		{
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void FIREBALL::Render()
{
	int ani = -1;
	switch (state)
	{

	case FIREBALL_STATE_FLYING:
				ani = FIREBALL_ANI_FLYING;
		
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void FIREBALL::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//case FIREBALL_STATE_FLYING:
	//	if (nx < 0) vx = -FIREBALL_SPEED;
	//	if (nx > 0) vx = FIREBALL_SPEED;
	//	break;

	}

}
