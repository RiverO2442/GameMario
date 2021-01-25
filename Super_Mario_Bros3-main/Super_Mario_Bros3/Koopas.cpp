#include "Koopas.h"



CKoopas::CKoopas(int ctype, int scene_id)
{
	type = ctype;
	nx = -1;
	if (type == KOOPAS_BLACK || scene_id == 1)
	{
		SetState(KOOPAS_STATE_SHELLING);
		isAppear = false;
	}
	else
	{
		if (type != KOOPAS_RED_FLY)
		{
			SetState(KOOPAS_STATE_WALKING);
		}
		else
		{
			SetState(KOOPAS_STATE_FLYING_UP_DOWN);
		}
	}
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

		if (dynamic_cast<CBreakableBrick*>(c->obj)->GetState() == BREAKABLE_BRICK_STATE_COIN)
		{
			ny = 0;
			nx = 0;
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

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBackGroundStage*>(obj))
		{
			CBackGroundStage* background_stage = dynamic_cast<CBackGroundStage*>(obj);
			if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
			{
				isAppear = true;
			}
		}
	}
	if (!isBeingHold && isAppear)
	{
		vy += KOOPAS_GRAVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!hitted)
		if (mario->HitByTail(this, KOOPAS_STATE_SHELLING, true))
		{
			hitted = true;
			if (type == KOOPAS_XANH_FLY)
			{
				type = KOOPAS_XANH_WALK;
			}
			SetShellUpRender(true);
			SetRenderRegconization(true);
			CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());
			playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, KOOPAS_VALUE);
			playscene->AddhitMng(this->x, this->y, HIT_EFFECT_TURN_TAIL);
			SetisAlive(false);
		}

	// turn off collision when die 
	if (state != KOOPAS_STATE_DIE || state != KOOPAS_STATE_DIE_2)
		CalcPotentialCollisions(coObjects, coEvents);

	if (isBeingHold == true) {
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
		if (mario->GetState() == MARIO_STATE_PIPE_DOWNING)
		{
			y = mario->y + MARIO_LEVEL_SMALL_KOOPAS_BONUS_Y;
			if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			{
				x = mario->x;
			}
			else
			{
				x = mario->x - 1;
				y = y - MARIO_LEVEL_SMALL_KOOPAS_BONUS_Y;
			}
			mario->SetrenderHolding(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			if (mario->nx > 0)
				x = mario->x + (MARIO_BIG_BBOX_WIDTH)*mario->nx;
			else x = mario->x + (MARIO_BIG_BBOX_WIDTH)*mario->nx;
		else x = mario->x + (KOOPAS_BBOX_WIDTH - 3) * mario->nx;
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			y = mario->y + 7;
		else y = mario->y - 2;
		vy = 0;
	}
	if ((DWORD)GetTickCount64() - jumpingStart >= KOOPAS_TIME_JUMPING && type == KOOPAS_XANH_FLY) // KOOPAS XANH FLY JUMP
	{
		vy = -GOOMBA_JUMP_SPEED;
		jumpingStart = (DWORD)GetTickCount64();
	}

	if (type == KOOPAS_RED_FLY)
	{

		if (state == KOOPAS_STATE_FLYING_UP_DOWN)
		{
			StartSwitchingState();
			if (isDown)
			{
				if (GetTickCount() - switching_state_time >= SWITCHING_STATE_TIME)
				{
					isDown = false;
					switching_state_time = 0;
				}
				else
				{
					vy = 0.06f;
				}
			}
			else
			{
				if (GetTickCount() - switching_state_time >= SWITCHING_STATE_TIME)
				{
					isDown = true;
					switching_state_time = 0;
				}
				else
				{
					vy = -0.06f;
				}
			}
		}
	}

	if (type != KOOPAS_BLACK)
	{
		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id != INTRO_SCENE_ID)
		{
			if (state == KOOPAS_STATE_SHELLING)
			{
				if (reviveStart == 0 || isKickedRevive)
				{
					StartRevive();
					isKickedRevive = false;
				}
			}
			if (mario->GetState() == MARIO_STATE_PIPE_DOWNING)
			{
				StartRevive();
			}
			//if (reviveStart != 0 && !isKickedRevive)
			//{
			//	if(state =! KOOPAS_STATE_SHELLING)
			//		StartRevive();
			//}
			if ((DWORD)GetTickCount64() - reviveStart >= REVIVE_START_TIME)
			{
				if (state == KOOPAS_STATE_SHELLING)
				{
					y -= REVIVE_START_Y;
					x += REVIVE_START_X * mario->nx;
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
				if ((DWORD)GetTickCount64() - reviveStart >= REVIVE_START_TIME_2)
				{
					reviveRender = true;
				}
			}
		}
		if (state != KOOPAS_STATE_WALKING)
			CanPullBack = false;
		if (state != KOOPAS_STATE_SHELLING && state != KOOPAS_STATE_SPINNING)
			shellUpRender = false;
		//int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id != 1)
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


			for (UINT i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				if (dynamic_cast<CQuestionBrick*>(obj))
				{
					CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(obj);
					if (this->x >= question_brick->x && this->x <= question_brick->x + QUESTION_BRICK_BBOX_WIDTH && question_brick->GetIsUp())
					{
						this->SetState(KOOPAS_STATE_SHELLING);
						this->SetShellUpRender(true);
						this->SetRenderRegconization(true);
						this->isKickedRevive = true;
						this->vy = -KOOPAS_DEFLECT_SPEED_Y;
						this->vx = KOOPAS_SHELL_DEFLECT_SPEED_VX * (-nx);
					}
				}
			}
		}

	}
	if (isAllowToSubRecWidth)
	{
		if ((DWORD)GetTickCount64() - TIMINGSUBRECWIDTH >= TIMINGSUBRECWIDTH)
		{
			isAllowToSubRecWidth = false;
		}
	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (CanPullBack && type == KOOPAS_RED_WALK)
		{
			if (y - CheckPosition_Y >= 1.0f)
			{

				y -= KOOPAS_CHECK_POSITION_MINUS_Y;
				if (vx < 0)
					x += KOOPAS_CHECK_POSITION_BONUS_X;
				else
					x -= KOOPAS_CHECK_POSITION_BONUS_X;
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
				hitted = false;
				if (state == KOOPAS_STATE_SHELLING)
					vx = 0;
			}
			if (dynamic_cast<CBreakableBrick*>(e->obj)) // if e->obj is Goomba
			{
				CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (state == KOOPAS_STATE_SPINNING && e->nx != 0)
				{
					breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj)) // if e->obj is Koopas
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
				if (state == KOOPAS_STATE_SPINNING)
				{
					if (Koopas->type == KOOPAS_XANH_FLY)
						Koopas->type = KOOPAS_XANH_WALK;
					Koopas->SetShellUpRender(true);
					Koopas->SetState(KOOPAS_STATE_DIE_2);
					Koopas->SetSpeed(this->nx * DIE_DEFLECT_SPEED_X, DIE_DEFLECT_SPEED_Y);
					Koopas->nx = (int)nx;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* Goomba = dynamic_cast<CGoomba*>(e->obj);

				vx = -vx;
				Goomba->vx = -Goomba->vx;
				if (state == KOOPAS_STATE_SPINNING)
				{
					Goomba->SetState(GOOMBA_STATE_DIE_2);
					Goomba->nx = (int)nx;
				}
			}
			else if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (state == KOOPAS_STATE_SPINNING)
					{
						int id = CGame::GetInstance()->GetCurrentScene()->GetId();
						if (id == SCENE_1_1_ID || id == SCENE_1_4_ID)
						{
							if (question_brick->GetIsAlive())
							{
								if (!question_brick->GetIsAllowQuestionBrickSlide())
								{
									question_brick->SetIsUp(true);
									question_brick->SetIsAlive(false);
									question_brick->SetIsAllowQuestionBrickSlide(true);
								}
							}
						}
						else if (id == 4)
						{
							if (question_brick->GetIsAlive())
							{
								if (question_brick->GetType() == QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE)
								{
									if (!question_brick->GetIsAllowQuestionBrickSlide())
									{
										question_brick->SetIsUp(true);
										question_brick->SetIsAllowToShowScore(true);
										question_brick->SetLifeDown();
										question_brick->SetIsAllowQuestionBrickSlide(true);
										question_brick->SetIsAllowToShowMultipleCoin(true);
										question_brick->SetControlMultipleCoin(false);
									}
								}
								else
								{
									if (!question_brick->GetIsAllowQuestionBrickSlide())
									{
										question_brick->SetIsUp(true);
										question_brick->SetIsAlive(false);
										question_brick->SetIsAllowToShowScore(true);
										question_brick->SetIsAllowQuestionBrickSlide(true);
									}
								}
							}

						}
					}
					vx = -vx;

				}
			}

			//Colli with any thing else then Koopas will change direction
			else if (nx != 0 && ny == 0)
			{
				nx = -nx;
				{
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					vx = -vx;
					if (id == SCENE_1_4_ID)
					{
						if (dynamic_cast<CBrick*>(e->obj) && this->GetState() == KOOPAS_STATE_SPINNING)
						{
							if (nx != 0 && ny == 0)
							{
								if (!this->isControlSubRecWidth)
								{
									this->isAllowToSubRecWidth = true;
									StartTimingSubRecWidth();
									this->isControlSubRecWidth = true;
								}
							}
						}
					}
				}
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
		if (dynamic_cast<CLeaf*>(e->obj) || (((dynamic_cast<CGoomba*>(e->obj) || dynamic_cast<CKoopas*>(e->obj)) && this->state != KOOPAS_STATE_SPINNING)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
}

void CKoopas::Render()
{
	int ani = -1;
	if (isAppear)
	{
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
		case KOOPAS_BLACK:
			if (state == KOOPAS_STATE_DIE)
			{
				ani = KOOPAS_BLACK_NGUA;
			}
			else if (state == KOOPAS_STATE_SHELLING)
			{
				ani = KOOPAS_BLACK_UP;
			}
			break;
		case KOOPAS_RED_FLY:
			ani = KOOPAS_RED_ANI_FLYING_LEFT_NEW;
			break;
		}
	}
	else return;
	if (ani == -1)
		return;
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
		vx = KOOPAS_STATE_SHELL_IDLE_SPEED;
		vy = KOOPAS_STATE_SHELL_IDLE_SPEED;
		break;
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL + 1;
		vx = KOOPAS_STATE_SHELL_IDLE_SPEED;
		vy = KOOPAS_STATE_SHELL_IDLE_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	}

}