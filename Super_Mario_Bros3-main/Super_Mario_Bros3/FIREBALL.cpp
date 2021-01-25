#include "FIREBALL.h"
#include <algorithm>
#include "Goomba.h"
#include "PlayScene.h"
#include "RECT.h"

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
	if ((DWORD)GetTickCount64() - reset_start > FIREBALL_RESET_TIME)
	{
		state = FIREBALL_STATE_DIE;
		reset_start = 0;
	}
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	// turn off collision when die 
	if (state != FIREBALL_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		isUsed = false;
		x = STORING_LOCATION_X;
		y = STORING_LOCATION_X;
		SetState(FIREBALL_STATE_FLYING);
	}
	if (y <= upBoudary)
		if (vy < 0)
			vy = -vy;
	if (isUsed == false)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetisFiring() == true)
		{
			if (mario->GetisAlreadyFired() == false)
			{
				isUsed = true;
				x = mario->x;
				y = mario->y;
				SetSpeed(mario->nx* FIRE_BALL_SPEED_VX, FIRE_BALL_SPEED_VY);
				mario->SetisAlreadyFired(true);
				upBoudary = mario->y;
				StartReset();
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
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* Goomba = dynamic_cast<CGoomba*>(e->obj);
				Goomba->SetState(GOOMBA_STATE_DIE_2);
				Goomba->SetSpeed(vx, -GOOMBA_DIE_DEFLECT_SPEED);
				CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
				playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, 100);
				playscene->AddhitMng(this->x, this->y, HIT_EFFECT_FIRE_BULLET);
				SetState(FIREBALL_STATE_DIE);
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
				if (Koopas->GetType() == KOOPAS_XANH_FLY)
					Koopas->SetType(KOOPAS_XANH_WALK);
				Koopas->SetState(KOOPAS_STATE_DIE_2);
				Koopas->SetSpeed(vx, KOOPAS_DEFLECT_SPEED_Y);
				CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
				playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, 100);
				playscene->AddhitMng(this->x, this->y, HIT_EFFECT_FIRE_BULLET);
				if (vx > 0)
					Koopas->nx = 1;
				else Koopas->nx = -1;
				SetState(FIREBALL_STATE_DIE);
			}
			else if (dynamic_cast<CFlower*>(e->obj))
			{
				CFlower* Koopas = dynamic_cast<CFlower*>(e->obj);
				Koopas->SetIsAlive(false);
				CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
				playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, 100);
				playscene->AddhitMng(this->x, this->y, HIT_EFFECT_FIRE_BULLET);
			}
			else //Colli with any thing else then Koopas will change direction
			if (ny != 0 )
			{
				if (ny > 0)
					upBoudary = y;
					vy = -vy;
			}
			if (nx != 0 )
			{
				SetState(FIREBALL_STATE_DIE);
			}
			
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		if (dynamic_cast<CRECT*>(e->obj))
		{
			if(dy < 0)
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
	if (state == FIREBALL_STATE_DIE)
		return;
	int ani;
	switch (state)
	{
	case FIREBALL_STATE_FLYING:
				ani = FIREBALL_ANI_FLYING;
				break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void FIREBALL::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREBALL_STATE_DIE:
		vx = FIRE_BALL_STATE_DIE_VX;
		vy = FIRE_BALLSTATE_DIE_VY;
		break;

	}

}
