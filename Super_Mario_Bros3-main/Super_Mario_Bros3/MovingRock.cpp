#include "MovingRock.h"



CMovingRock::CMovingRock()
{
	SetState(MOVING_ROCK_STATE_IDLE);
	SetPosition(STORING_LOCATION_X, STORING_LOCATION_Y);
}




void CMovingRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CMovingRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	if (isUsed)
		if ((DWORD)GetTickCount64() - timing_start >= MOVING_ROCK_DELAY_TIME_GRAVITY)
			vy += MOVING_ROCK_GRAVITY * dt;


	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBreakableBrick*>(obj))
		{
			CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*>(obj);
			if (breakable_brick->GetisBreaking())
			{
				if (breakable_brick->Getcount() <= 3)
				{
					if (!isUsed)
					{
						breakable_brick->Setcountup();
						SetState(breakable_brick->Getcount());
						this->SetPosition(breakable_brick->GetbreakX() + (breakable_brick->Getcount() % 2) * MOVING_ROCK_PTS_FIX, breakable_brick->GetbreakY() + (breakable_brick->Getcount() / 3) * MOVING_ROCK_PTS_FIX);
						isUsed = true;
						StartTiming();
					}
				}
				else (breakable_brick->SetisBreaking(false));
			}
		}
	}

	if (isUsed)
	{
		if ((DWORD)GetTickCount64() - timing_start >= MOVING_ROCK_DISAPEAR_TIME_LIMIT)
		{
			isUsed = false;
			SetPosition(STORING_LOCATION_X, STORING_LOCATION_Y);
			timing_start = 0;
			state = MOVING_ROCK_STATE_IDLE;
		}
	}

	if (isUsed)
		if ((DWORD)GetTickCount64() - timing_start >= MOVING_ROCK_DELAY_TIME_MOVING)
		{
			x += dx;
			y += dy;
		}
}

void CMovingRock::Render()
{
	if (isUsed)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;


	//RenderBoundingBox();
}

void CMovingRock::SetState(int state)
{
	vy = (float)-0.04;
	CGameObject::SetState(state);
	switch (state)
	{
	case  MOVING_ROCK_STATE_IDLE:
		vx = MOVING_ROCK_STATE_IDLE_VX;
		vy = MOVING_ROCK_STATE_IDLE_VY;
		break;
	case MOVING_ROCK_STATE_TOP_RIGHT:
		vx = MOVING_ROCK_VX;
		vy = -MOVING_ROCK_VY;
		break;
	case MOVING_ROCK_STATE_TOP_LEFT:
		vx = -MOVING_ROCK_VX;
		vy = -MOVING_ROCK_VY;
		break;
	case MOVING_ROCK_STATE_BOTTOM_RIGHT:
		vx = MOVING_ROCK_VX;
		break;
	case MOVING_ROCK_STATE_BOTTOM_LEFT:
		vx = -MOVING_ROCK_VX;
		break;
	}
}

