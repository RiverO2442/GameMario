#include "Koopas.h"
#include <algorithm>
#include "Goomba.h"
#include "PlayScene.h"
#include "RECT.h"
#include "FIREBALL.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
}
CKoopas::CKoopas(int ctype)
{
	type = ctype;
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
}

void CKoopas::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CMario*>(c->obj))
		{
			ny = -0.01f;
		}
	}
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_DIE_2)
		return;
	else if (state == KOOPAS_STATE_SHELLING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else if (state == KOOPAS_STATE_SPINNING)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else if (state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE || state != KOOPAS_STATE_DIE_2)
		CalcPotentialCollisions(coObjects, coEvents);
	if (isBeingHold == true) {
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetisHolding() == false)
		{
			isBeingHold = false;
			mario->SetIsKicking(true);
			mario->StartKicking();
			if (mario->nx > 0)
			{
				SetState(KOOPAS_STATE_SPINNING);
				nx = 1;
				vx = KOOPAS_SPINNING_SPEED;
			}
			else if (mario->nx < 0)
			{
				SetState(KOOPAS_STATE_SPINNING);
				nx = -1;
				vx = -KOOPAS_SPINNING_SPEED;
			}
			mario->SetrenderHolding(false);
		}
		else mario->SetrenderHolding(true);
		if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			if (mario->nx > 0)
				x = mario->x + (MARIO_BIG_BBOX_WIDTH + 5) * mario->nx;
			else x = mario->x + (MARIO_BIG_BBOX_WIDTH)*mario->nx;
		else x = mario->x + (KOOPAS_BBOX_WIDTH - 3) * mario->nx;
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			y = mario->y + 7;
		else y = mario->y - 2;
		vy = 0;
	}
	if (GetTickCount() - jumpingStart >= KOOPAS_TIME_JUMPING && type == KOOPAS_XANH_FLY) // KOOPAS XANH FLY JUMP
	{
		vy = -GOOMBA_JUMP_SPEED;
		jumpingStart = GetTickCount();

	}

	if (state == KOOPAS_STATE_SHELLING)
	{
		if (reviveStart == 0 || isKickedRevive)
		{
			StartRevive();
			isKickedRevive = false;
		}
	}

	if (GetTickCount() - reviveStart >= 5000)
	{
		if (state == KOOPAS_STATE_SHELLING)
		{
			y -= 10;
			x += 5 * mario->nx;
			SetState(KOOPAS_STATE_WALKING);
			if (mario->x >= this->x)
				vx = -vx;
			isBeingHold = false;
			mario->SetrenderHolding(false);
		}
		reviveStart = 0;
		reviveRender = false;
	}
	else
	{
		if (GetTickCount() - reviveStart >= 3000)
		{
			reviveRender = true;
		}
	}

	if (state != KOOPAS_STATE_WALKING)
		CanPullBack = false;
	if (state != KOOPAS_STATE_SHELLING && state != KOOPAS_STATE_SPINNING)
		shellUpRender = false;
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (CanPullBack && type == KOOPAS_RED_WALK)
		{
			if (y - CheckPosition_Y >= 1.0f)
			{

				y -= 5;
				if (vx < 0)
					x += 12;
				else
					x -= 12;
				vx = -vx;
			}
		}
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

		//if(nx != 0 && ny != 0)
		y += min_ty * dy + ny * 0.4f;



		if (ny != 0) vy = 0;

		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (!dynamic_cast<CMario*>(e->obj) && nx == 0)
			{
				CheckPosition_Y = y;
				CanPullBack = true;
			}
			if (e->ny < 0)
			{
				if (state == KOOPAS_STATE_SHELLING)
					vx = 0;
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
				if (state == KOOPAS_STATE_SPINNING)
				{
					Koopas->SetState(KOOPAS_STATE_DIE_2);
					Koopas->nx = nx;
				}
				vx = -vx;
				Koopas->vx = -Koopas->vx;
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* Goomba = dynamic_cast<CGoomba*>(e->obj);
				if (state == KOOPAS_STATE_SPINNING)
				{
					Goomba->SetState(GOOMBA_STATE_DIE_2);
					Goomba->nx = nx;
				}
				vx = -vx;
				Goomba->vx = -Goomba->vx;
			}
			//Colli with any thing else then Koopas will change direction
			else if (nx != 0 && ny == 0)
			{
				nx = -nx;
				vx = -vx;
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		if (x <= 0)
		{
			if (vx < 0)
				vx = -vx;
		}
	}
}

void CKoopas::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CMario*>(e->obj))
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->GetUnTounchable() == 1)
				continue;
		}
		if (dynamic_cast<CRECT*>(e->obj))
		{
			if (!(e->ny < 0))
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CKoopas::Render()
{
	int ani = -1;
	switch (type)
	{
	case KOOPAS_XANH_WALK:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_ANI_SHELL_UP;
			if (vx > 0)
				ani = KOOPAS_ANI_WALKING_RIGHT;
			else
				ani = KOOPAS_ANI_WALKING_LEFT;
		}
		else if (state == KOOPAS_STATE_SHELLING)
		{
			if (reviveRender)
			{
				if (shellUpRender)
					ani = KOOPAS_XANH_ANI_REVIVING_NGUA;
				else
					ani = KOOPAS_XANH_ANI_REVIVING;
			}
			else if (shellUpRender)
			{
				ani = KOOPAS_ANI_SHELL_UP;
			}
			else
				ani = KOOPAS_ANI_SHELL_DOWN;
		}
		else if (state == KOOPAS_STATE_DIE_2)
		{
			ani = KOOPAS_ANI_SHELL_UP;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			if (shellUpRender)
			{
				ani = KOOPAS_ANI_SPINNING_UP;
			}
			else
				ani = KOOPAS_ANI_SPINNING;
		}
		else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else  ani = KOOPAS_ANI_WALKING_LEFT;
		break;
	case KOOPAS_XANH_FLY:
		if (vx < 0) ani = KOOPAS_ANI_FLYING_LEFT;
		else  ani = KOOPAS_ANI_FLYING_LEFT;
		break;

	case KOOPAS_RED_WALK:
		if (state == KOOPAS_STATE_SHELLING)
		{
			if (reviveRender)
			{
				if (shellUpRender)
					ani = KOOPAS_RED_ANI_REVIVING_NGUA;
				else
					ani = KOOPAS_RED_ANI_REVIVING;
			}
			else if (shellUpRender)
			{
				ani = KOOPAS_RED_ANI_SHELL_UP;
			}
			else
				ani = KOOPAS_RED_ANI_SHELL_DOWN;
		}
		else if (vx > 0) ani = KOOPAS_RED_ANI_WALKING_RIGHT;
		else ani = KOOPAS_RED_ANI_WALKING_LEFT;
		if (state == KOOPAS_STATE_SPINNING)
		{
			if (shellUpRender)
			{
				ani = KOOPAS_RED_ANI_SPINNING_UP;
			}
			else
				ani = KOOPAS_RED_ANI_SPINNING;
		}
		DebugOut(L"[INFO] Done loading scene resources %d\n", ani);
		break;

	case KOOPAS_RED_FLY:
		if (state == KOOPAS_STATE_SHELLING) {
			ani = KOOPAS_RED_ANI_SHELL_DOWN;
		}
		else if (vx < 0) ani = KOOPAS_RED_ANI_FLYING_LEFT;
		else  ani = KOOPAS_RED_ANI_FLYING_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SPINNING:
		if (nx < 0) vx = -KOOPAS_SPINNING_SPEED;
		if (nx > 0) vx = KOOPAS_SPINNING_SPEED;
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
		vx = -KOOPAS_WALKING_SPEED;
	}

}