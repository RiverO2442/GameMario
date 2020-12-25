#include "MovingRock.h"



CMovingRock::CMovingRock()
{
	SetState(MOVING_ROCK_STATE_IDLE);
	SetPosition(12000, 12000);
}




void CMovingRock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CMovingRock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	if (isUsed)
		if (GetTickCount() - timing_start >= 100)
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
						this->SetPosition(breakable_brick->GetbreakX() + (breakable_brick->Getcount() % 2) * 8, breakable_brick->GetbreakY() + (breakable_brick->Getcount() / 3) * 8);
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
		if (GetTickCount() - timing_start >= 1200)
		{
			isUsed = false;
			SetPosition(12000, 12000);
			timing_start = 0;
			state = MOVING_ROCK_STATE_IDLE;
		}
	}

	if (isUsed)
		if (GetTickCount() - timing_start >= 50)
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
	vy = -0.04;
	CGameObject::SetState(state);
	switch (state)
	{
	case  MOVING_ROCK_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MOVING_ROCK_STATE_TOP_RIGHT:
		vx = 0.08;
		vy = -0.08;
		break;
	case MOVING_ROCK_STATE_TOP_LEFT:
		vx = -0.08;
		vy = -0.08;
		break;
	case MOVING_ROCK_STATE_BOTTOM_RIGHT:
		vx = 0.08;
		break;
	case MOVING_ROCK_STATE_BOTTOM_LEFT:
		vx = -0.08;
		break;
	}
}

