#include "Coin.h"
#include "Mario.h"

CCoin::CCoin(int ctype)
{
	type = ctype;
	if (type == COIN_NORMAL)
	{
		isAppear = true;
	}
	else
		isAppear = false;
	SetState(COIN_STATE_IDLE);
}




void CCoin::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (!dynamic_cast<CMario*>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick*>(obj))
		{
			CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(obj);
			int id = CGame::GetInstance()->GetCurrentScene()->GetId();
			if (id == 3)
			{
				if (!question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF && !question_brick->GetIsUsed())
				{
					if (!isAppear && type == COIN_CAN_MOVE)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(COIN_STATE_UP);
							StartTiming();
							isAppear = true;
							CGame::GetInstance()->MoneyUp();
							question_brick->SetIsUsed(true);
						}
					}

				}
			}
			else if (id == 4)
			{
				if (question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE && !question_brick->GetControlMultipleCoin() && question_brick->GetIsAllowToShowMultipleCoin())
				{
					if (!isAppear && type == COIN_CAN_MOVE)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(COIN_STATE_UP);
							StartTiming();
							isAppear = true;
							CGame::GetInstance()->MoneyUp();
							question_brick->SetControlMultipleCoin(true);
						}
					}
				}
			}
		}
	}





	if (state == COIN_STATE_UP)
	{
		if ((DWORD)GetTickCount64() - timing_start >= COIN_STATE_UP_TIME_UP)
		{
			SetState(COIN_STATE_DOWN);
			StartTiming();
		}
	}

	if (state == COIN_STATE_DOWN)
	{
		if ((DWORD)GetTickCount64() - timing_start >= COIN_STATE_DOWN_TIME_DOWN)
		{
			isAppear = false;
			CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
			playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, MOVING_COIN_VALUE);
			CGame::GetInstance()->ScoreUp(100);
			SetState(COIN_STATE_IDLE);
		}
	}

	x += dx;
	y += dy;
}

void CCoin::Render()
{
	if (!isAppear)
		return;

	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}



void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!isAppear || type == COIN_CAN_MOVE)
		return;
	else
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_IDLE:
		vx = vy = COIN_STATE_IDLE_SPEED;
		break;
	case COIN_STATE_UP:
		vy = -COIN_STATE_UP_SPEED;
		break;
	case COIN_STATE_DOWN:
		vy = COIN_STATE_DOWN_SPEED;
		break;
	}
}