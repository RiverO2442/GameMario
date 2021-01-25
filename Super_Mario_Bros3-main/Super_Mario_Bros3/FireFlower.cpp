#include "FireFlower.h"

CFireFlower::CFireFlower()
{
	SetState(FIRE_FLOWER_STATE_IDLE);
}

void CFireFlower::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
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
void CFireFlower::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isAppear)
	{
		l = x;
		t = y;
		r = x + FIRE_FLOWER_BBOX_WIDTH;
		b = y + FIRE_FLOWER_BBOX_HEIGHT;

	}
	else
	{
		l = t = r = b = 0;
	}
}

void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAppear)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick*>(obj))
		{
			CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(obj);
			if (!question_brick->GetIsAlive() && !question_brick->GetIsUsed() && !question_brick->GetIsUsed())
			{
				if (mario->GetLevel() == MARIO_LEVEL_TAIL || mario->GetLevel() == MARIO_LEVEL_FIRE)
				{
					if (!isAppear)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(FIRE_FLOWER_STATE_UP);
							isAppear = true;
							StartUpping();
							question_brick->SetIsUsed(true);
						}
					}
				}
			}
		}

	}


	if (state == FIRE_FLOWER_STATE_UP)
	{
		if ((DWORD)GetTickCount64() - upping_start >= COIN_STATE_UP_TIME_UP)
		{
			SetState(FIRE_FLOWER_STATE_IDLE);
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

		int id = CGame::GetInstance()->GetCurrentScene()->GetId();

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				if (mario->GetLevel() == MARIO_LEVEL_TAIL)
				{
					SetPosition(STORING_LOCATION_X, STORING_LOCATION_Y);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_FIRE)
				{
					isAppear = false;
					SetPosition(STORING_LOCATION_X, STORING_LOCATION_Y);
				}
				int id = CGame::GetInstance()->GetCurrentScene()->GetId();
				if (id == SCENE_1_1_ID || id == SCENE_1_4_ID)
				{
					CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
					playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, 1000);
				}
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CFireFlower::Render()
{

	if (isAppear)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;

	//RenderBoundingBox();
}

void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = vy = COIN_STATE_IDLE_SPEED;
		break;
	case  FIRE_FLOWER_STATE_UP:
		vy = -COIN_STATE_UP_SPEED;
		break;
	}
}


