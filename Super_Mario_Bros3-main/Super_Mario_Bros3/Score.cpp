#include "Score.h"



CScore::CScore()
{
	isUsed = false;
	SetState(SCORE_STATE_IDLE);
}


void CScore::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

	if (playscene->CheckScoreMng() && !isUsed)
	{
		this->SetPosition(playscene->GetScoreManager()->getScorePoisitionX(), playscene->GetScoreManager()->getScorePoisitionY());
		value = playscene->GetScoreManager()->getScore();
		playscene->DeleteScore();
		isUsed = true;
		SetState(SCORE_STATE_UP);
		StartTiming();
	}

	if (state == SCORE_STATE_UP)
	{
		if (GetTickCount() - timing_start >= 600)
		{
			isUsed = false;
			SetPosition(6000, 6000);
			SetState(SCORE_STATE_IDLE);
			timing_start = 0;
			if(value != 0)
			CGame::GetInstance()->ScoreUp(value);
			else
			{
				CGame::GetInstance()->SetLifeUp();
			}
			value = 0;
		}
	}

	x += dx;
	y += dy;
}

void CScore::Render()
{
	int ani = -1;
	if (isUsed && value != 0)
	{
		switch (value)
		{
		case SCORE_TYPE_100:
			ani = SCORE_100_ANI;
			break;
		case SCORE_TYPE_200:
			ani = SCORE_200_ANI;
			break;
		case SCORE_TYPE_400:
			ani = SCORE_400_ANI;
			break;
		case SCORE_TYPE_800:
			ani = SCORE_800_ANI;
			break;
		case SCORE_TYPE_1000:
			ani = SCORE_1000_ANI;
			break;
		case SCORE_TYPE_2000:
			ani = SCORE_2000_ANI;
			break;
		case SCORE_TYPE_4000:
			ani = SCORE_4000_ANI;
			break;
		case SCORE_TYPE_8000:
			ani = SCORE_8000_ANI;
			break;
		case SCORE_TYPE_1LIFE:
			ani = SCORE_1LIFE_ANI;
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CScore::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  SCORE_STATE_IDLE:
		vx = vy = 0;
		break;
	case  SCORE_STATE_UP:
		vy = -0.08f;
		break;
	}
}

