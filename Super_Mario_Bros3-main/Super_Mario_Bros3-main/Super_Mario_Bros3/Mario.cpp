#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "PlayScene.h"
#include "RECT.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny < 0)
				SetIsJumping(0);
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CRECT*>(e->obj))
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->dy < 0)
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else

		//BIG MARIO


		if (level == MARIO_LEVEL_BIG)
		{
			if (GetState() == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			else if (GetState() == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (GetState() == MARIO_STATE_RUNNING_LEFT)
				ani = MARIO_ANI_BIG_RUNNING_LEFT;
			else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
				ani = MARIO_ANI_BIG_RUNNING_RIGHT;
			else if (GetState() == MARIO_STATE_HOLDING)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_RIGHT;
				else ani = MARIO_ANI_BIG_HOLDING_LEFT;
			}
			else if (GetState() == MARIO_STATE_KICK)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
				else ani = MARIO_ANI_BIG_KICKING_LEFT;
			}
			else if (GetState() == MARIO_STATE_FLYING_LEFT)
				ani = MARIO_ANI_BIG_FLYING_LEFT;
			else if (GetState() == MARIO_STATE_FLYING_RIGHT)
				ani = MARIO_ANI_BIG_FLYING_RIGHT;
			else if (GetState() == MARIO_STATE_BRAKING_LEFT)
				ani = MARIO_ANI_BIG_BRAKING_LEFT;
			else if (GetState() == MARIO_STATE_BRAKING_RIGHT)
				ani = MARIO_ANI_BIG_BRAKING_RIGHT;
			else if (GetState() == MARIO_STATE_SITDOWN)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
				else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
			}
			else if (GetState() == MARIO_STATE_IDLE)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			if (//GetState() == MARIO_STATE_JUMP)
				GetIsJumping() == true)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
				else ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
		}

	//SMALL MARIO

		else if (level == MARIO_LEVEL_SMALL)
		{
			if (GetState() == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			else if (GetState() == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else if (GetState() == MARIO_STATE_RUNNING_LEFT)
				ani = MARIO_ANI_SMALL_RUNNING_LEFT;
			else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
				ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
			else if (GetState() == MARIO_STATE_HOLDING)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLDING_LEFT;
			}
			else if (GetState() == MARIO_STATE_SITDOWN)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (GetState() == MARIO_STATE_KICK)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
				else ani = MARIO_ANI_SMALL_KICKING_LEFT;
			}
			else if (GetState() == MARIO_STATE_FLYING_LEFT)
				ani = MARIO_ANI_SMALL_FLYING_LEFT;
			else if (GetState() == MARIO_STATE_FLYING_RIGHT)
				ani = MARIO_ANI_SMALL_FLYING_RIGHT;
			else if (GetState() == MARIO_STATE_BRAKING_LEFT)
				ani = MARIO_ANI_SMALL_BRAKING_LEFT;
			else if (GetState() == MARIO_STATE_BRAKING_RIGHT)
				ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
			//else if (GetState() == MARIO_STATE_SITDOWN)
			//{
			//	if (nx > 0) ani = MARIO_ANI_SMALL_SITDOWN_RIGHT;
			//	else ani = MARIO_ANI_SMALL_SITDOWN_LEFT;
			//} MARIO SMALL KO CO STATE NGOI
			else if (GetState() == MARIO_STATE_IDLE)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			if (//GetState() == MARIO_STATE_JUMP)
				GetIsJumping() == true)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
			}
		}

	// TAIL MARIO

		else if (level == MARIO_LEVEL_TAIL)
		{
			if (GetState() == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_TAIL_WALKING_LEFT;
			else if (GetState() == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_TAIL_WALKING_RIGHT;
			else if (GetState() == MARIO_STATE_RUNNING_LEFT)
				ani = MARIO_ANI_TAIL_RUNNING_LEFT;
			else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
				ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
			else if (GetState() == MARIO_STATE_HOLDING)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_HOLDING_RIGHT;
				else ani = MARIO_ANI_TAIL_HOLDING_LEFT;
			}
			else if (GetState() == MARIO_STATE_KICK)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
				else ani = MARIO_ANI_TAIL_KICKING_LEFT;
			}
			else if (GetState() == MARIO_STATE_FLYING_LEFT)
				ani = MARIO_ANI_TAIL_FLYING_LEFT;
			else if (GetState() == MARIO_STATE_FLYING_RIGHT)
				ani = MARIO_ANI_TAIL_FLYING_RIGHT;
			else if (GetState() == MARIO_STATE_BRAKING_LEFT)
				ani = MARIO_ANI_TAIL_BRAKING_LEFT;
			else if (GetState() == MARIO_STATE_BRAKING_RIGHT)
				ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
			else if (GetState() == MARIO_STATE_SITDOWN)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
				else ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
			}
			else if (GetState() == MARIO_STATE_IDLE)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
				else ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
			if (//GetState() == MARIO_STATE_JUMP)
				GetIsJumping() == true)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
				else ani = MARIO_ANI_TAIL_JUMPING_LEFT;
			}
		}

	// FIRE MARIO

		else if (level == MARIO_LEVEL_FIRE)
		{
			if (GetState() == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
			else if (GetState() == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else if (GetState() == MARIO_STATE_RUNNING_LEFT)
				ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
				ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
			else if (GetState() == MARIO_STATE_HOLDING)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLDING_LEFT;
			}
			else if (GetState() == MARIO_STATE_KICK)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
				else ani = MARIO_ANI_FIRE_KICKING_LEFT;
			}
			else if (GetState() == MARIO_STATE_FLYING_LEFT)
				ani = MARIO_ANI_FIRE_FLYING_LEFT;
			else if (GetState() == MARIO_STATE_FLYING_RIGHT)
				ani = MARIO_ANI_FIRE_FLYING_RIGHT;
			else if (GetState() == MARIO_STATE_BRAKING_LEFT)
				ani = MARIO_ANI_FIRE_BRAKING_LEFT;
			else if (GetState() == MARIO_STATE_BRAKING_RIGHT)
				ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
			else if (GetState() == MARIO_STATE_SITDOWN)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
				else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
			}
			else if (GetState() == MARIO_STATE_IDLE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			if (//GetState() == MARIO_STATE_JUMP)
				GetIsJumping() == true)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
			}
		}
	
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	/*case MARIO_STATE_ON_FIRE:
		SetLevel(MARIO_LEVEL_FIRE);
		break;*/
	case MARIO_STATE_WALKING_RIGHT:
		if(vx < 0.2)
		vx = vx + MARIO_WALKING_GIATOC * dt;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		vx = 5*MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		vx = -5 * MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == MARIO_STATE_SITDOWN )
	{
		if(level != MARIO_LEVEL_SMALL)
		top = top + 10;
	}
	if (level == MARIO_LEVEL_FIRE)
	{	
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_BIG_BBOX_WIDTH + 7;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::FIXPS(int PSx, int PSy)
{
	if (level != MARIO_LEVEL_SMALL)
	{
		x = x + PSx;
		y = y + PSy;
	}
}

void CMario::LvChanging()
{
	if (level == MARIO_LEVEL_SMALL)
	{
		y = y - MARIO_BBOX_HEIGHT_DIFF;
	}
}
