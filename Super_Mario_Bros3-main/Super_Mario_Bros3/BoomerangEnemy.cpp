#include "BoomerangEnemy.h"

CBoomerangEnemy::CBoomerangEnemy()
{
	SetState(BOOMERANG_ENEMY_STATE_IDLE);
	nx = 1;
}

void CBoomerangEnemy::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));


		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CBoomerangEnemy::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isAllowToHaveBBox)
	{
		l = x;
		t = y;
		r = x + BOOMERANG_ENEMY_BBOX_WIDTH;
		b = y + BOOMERANG_ENEMY_BBOX_HEIGHT;
	}
	else return;


}

void CBoomerangEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	if (pre_get_tick_count == 0)
		pre_get_tick_count = (DWORD)GetTickCount64();
	else
	{
		if ((DWORD)GetTickCount64() - pre_get_tick_count <= SAFE_TIME)
		{
			pre_get_tick_count = (DWORD)GetTickCount64();
		}
		else
		{
			sub_time += (DWORD)GetTickCount64() - pre_get_tick_count;
			pre_get_tick_count = (DWORD)GetTickCount64();
		}
	}

	// Simple fall down

	vy += BOOMERANG_ENEMY_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAlive)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();



	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id != INTRO_SCENE_ID)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetIsSpining())
		{
			if (mario->HitByTail(this, BOOMERANG_ENEMY_STATE_DIE, true))
			{
				CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
				playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, 1000);
			}
		}
	}


	if (mario->x - this->x >= BOOMERANG_ENEMY_LOOK_RIGTH_LIMIT)
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = 1;
		}
	}
	else if(mario->x - this->x < BOOMERANG_ENEMY_LOOK_LEFT_LIMIT)
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = -1;
		}
	}


	if (isAlive)
	{

		if ((DWORD)GetTickCount64() - time_rendering_throw_ani >= RENDER_THROW_ANI_TIME_LIMIT)
		{
			isAllowToRenderThrowAni = false;
			time_rendering_throw_ani = 0;
		}


		StartTimeSwitchingState();

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<CBoomerang*>(obj))
			{
				CBoomerang* boomerang = dynamic_cast<CBoomerang*>(obj);
				int boomerang_id = boomerang->GetId();
				if (boomerang_id == 1)
				{
					if (!boomerang->GetIsAllowToThrowBoomerang())
					{
						if ((DWORD)GetTickCount64() - time_switch_state >= RENDER_THROW_TIME_LIMIT + sub_time)
						{
							if (this->nx > 0)
								boomerang->SetBoomerangDirection(1);
							else
								boomerang->SetBoomerangDirection(-1);
							boomerang->SetIsAllowToThrowBoomerang(true);
							boomerang->SetIsInState_1(true);
							isAllowToRenderThrowAni = true;
							StartTimeRenderingThrowAni();
							boomerang->StartTimeSwitchingState();
						}
					}
				}
				else
				{
					if (!boomerang->GetIsAllowToThrowBoomerang())
					{
						if ((DWORD)GetTickCount64() - time_switch_state >= RENDER_SECOND_THROW_TIME_LIMIT + sub_time)
						{
							if (this->nx > 0)
								boomerang->SetBoomerangDirection(1);
							else
								boomerang->SetBoomerangDirection(-1);
							boomerang->SetIsAllowToThrowBoomerang(true);
							boomerang->SetIsInState_1(true);
							isAllowToRenderThrowAni = true;
							StartTimeRenderingThrowAni();
							boomerang->StartTimeSwitchingState();
						}
					}
					//DebugOut(L"da vao day \n");
				}

			}
		}


		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_MOVE_FORWARD_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_FORWARD);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_IDLE_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_IDLE);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_MOVE_BACKWARD_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_BACKWARD);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_IDLE_2_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_IDLE);
			time_switch_state = 0;
			sub_time = 0;
		}

	}
	else
	{
		SetState(BOOMERANG_ENEMY_STATE_DIE);
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		//vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;



		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBoomerangEnemy::Render()
{
	int ani = -1;

	if (isAlive)
	{
		if (isAllowToRenderThrowAni)
		{
			if (nx > 0)
				ani = BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_RIGHT;
			else
			{
				ani = BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_LEFT;
			}
		}
		else
		{
			if (nx > 0)
				ani = BOOMERANG_ENEMY_ANI_NORMAL_RIGHT;
			else
			{
				ani = BOOMERANG_ENEMY_ANI_NORMAL_LEFT;
			}
		}
	}
	else
	{
		if (nx > 0)
			ani = BOOMERANG_ENEMY_ANI_DIE_RIGHT;
		else
			ani = BOOMERANG_ENEMY_ANI_DIE_LEFT;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBoomerangEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case  BOOMERANG_ENEMY_STATE_IDLE:
		vx = vy = 0;
		break;
	case  BOOMERANG_ENEMY_STATE_MOVE_FORWARD:
		vx = BOOMETANG_ENEMY_VX;
		vy = BOOMETANG_ENEMY_VY;
		break;
	case  BOOMERANG_ENEMY_STATE_MOVE_BACKWARD:
		vx = -BOOMETANG_ENEMY_VX;
		vy = BOOMETANG_ENEMY_VY;
		break;
	case BOOMERANG_ENEMY_STATE_DIE:
		vx = BOOMETANG_ENEMY_VX;
	}
}


