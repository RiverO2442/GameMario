#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "PlayScene.h"
#include "RECT.h"
#include "Koopas.h"
#include "FIREBALL.h"
#include "Coin.h"
#include "QuestionBrick.h"
#include "IntroScence.h"

CMario::CMario(int ctype, float x, float y) : CGameObject()
{
	type = ctype;
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_WALKING_RIGHT);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	isAppear = true;
}
void CMario::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
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
		if (dynamic_cast<CCoin*>(c->obj))
		{
			nx = 0;
			ny = 0;
		}

		if (dynamic_cast<CMushRoom*>(c->obj) || dynamic_cast<CLeaf*>(c->obj) || dynamic_cast<CFlowerBullet*>(c->obj) || dynamic_cast<CKoopas*>(c->obj) || dynamic_cast<CGoomba*>(c->obj) || dynamic_cast<CMario*>(c->obj))
		{
			if (ny != 0)
				ny = -0.0001f;
		}
		if (dynamic_cast<CBreakableBrick*>(c->obj))
		{
			CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*> (c->obj);
			if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_COIN)
			{
				if (isJumping)
				{
					nx = 0;
					ny = 0;
				}
			}
		}
		if (dynamic_cast<CRECT*>(c->obj))
		{
			if (ny > 0)//|| nx != 0)
			{
				nx = 0;
				ny = 0;
			}
		}
	}//

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);


	// Simple fall down
	if (state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING && !isTransform && !isSmokeTransform)
		vy += MARIO_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isTransform || isSmokeTransform)
	{
		vx = 0;
		vy = 0;
	}
	if (state != MARIO_STATE_DIE && state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING)
		CalcPotentialCollisions(coObjects, coEvents);

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == SCENE_1_1_ID)
	{
		if (state == MARIO_STATE_DIE && GetcanSetLifeDown())
		{
			StartSwitchScene();
			CGame::GetInstance()->LifeDown();
			SetcanSetLifeDown(false);
		}
		if(switch_scene_start != 0)
		if (state == MARIO_STATE_DIE && GetTickCount() - switch_scene_start >= 2000)
		{
			switch_scene_start = 0;
			switch_scene = true;
		}
			
		int time_picker = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetTimePicker();

		if (time_picker == 0 && state != MARIO_STATE_DIE && switch_scene_start == 0)
		{
			SetState(MARIO_STATE_DIE);
		}
		// turn off collision when die 
		if (switch_scene_start != 0 && state != MARIO_STATE_DIE)
		{
			if (GetTickCount() - switch_scene_start >= 1000)
			{
				this->isAllowToShowWordsEndScene = true;
			}
			if (GetTickCount() - switch_scene_start >= 3000)
			{
				StartCountDownTimePicker();
				if (GetTickCount() - count_down_time_start >= 50)
				{
					if (time_picker > 0)
					{
						((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SetTimeDown();
						count_down_time_start = 0;
					}
				}

			}
			if (GetTickCount() - switch_scene_start >= 6000)
			{
				if (time_picker == 0 && timetemp != 0)
				{
					CGame::GetInstance()->ScoreUp(TIME_VALUE * timetemp);
					timetemp = 0;
				}
			}

			if (GetTickCount() - switch_scene_start >= 8000)
			{
				switch_scene_start = 0;
				switch_scene = true;
			}

			DebugOut1(L"[INFO] level La : %d \n", GetTickCount() - switch_scene_start);
		}
			
			if (state == MARIO_STATE_PIPE_DOWNING)
			{
				if (GetTickCount() - pipe_downing_start >= 3000)
				{
					this->SetPosition(1330, 1050);
					canPipeDowning = false;
					isAtTheTunnel = true;
					SetState(MARIO_STATE_IDLE);
					pipe_downing_start = 0;
				}
				DEACCELERETING();
			}

			if (state == MARIO_STATE_PIPE_UPPING)
			{
				if (GetTickCount() - pipe_upping_start >= 3100)
				{
					isAtTheTunnel = false;
					if (!setPositionOutOfTunnel)
					{
						this->SetPosition(2330, 122);
						setPositionOutOfTunnel = true;
					}
				}

				if (GetTickCount() - pipe_upping_start >= 6350)
				{
					canPipeUpping = false;
					SetState(MARIO_STATE_IDLE);
					pipe_upping_start = 0;
					setPositionOutOfTunnel = false;
				}
				
				DEACCELERETING();
			}
			// reset untouchable timer if untouchable time has passed
			if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
			{
				untouchable_start = 0;
				untouchable = 0;
			}

			if (GetTickCount() - firing_start > MARIO_FIRING_TIME)
			{
				isFiring = 0;
				firing_start = 0;
			}

			if (GetTickCount() - transform_start > MARIO_TRANSFORM_TIME)
			{
				transform_start = 0;
				isTransform = false;
				isSmokeTransform = false;
			}

			if (GetTickCount() - kicking_start > MARIO_KICKING_TIME)
			{
				SetIsKicking(false);
			}


			if (GetTickCount() - spining_start > MARIO_SPINING_TIME)
			{
				SetIsSpining(false);
			}

			if (flying_start != 0)
			{
				if (GetTickCount() - flying_start >= 7000)
				{
					canFly = false;
					isFlying = false;
					flying_start = 0;
					speedLevel = 0;
				}
			}

		if (abs(y - heightLimit) >= 1)
			isJumping = true;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		heightLimit = y;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		if (isFlying)
			ny = -0.004;

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		if (ny < 0)    // Handle Jumping
		{
			if (vx == 0)
				control_able = 0;
			isJumping = false;
			isFalling = false;
			canFall = false;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			if (ny < 0)
			{
				SetIsJumping(0);
				SetCanFly(0);
				flying_start = 0;
			}
				
			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Coin
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (coin->GetType() == COIN_NORMAL)
				{
					coin->SetIsAppear(false);
					CGame::GetInstance()->MoneyUp();
				}
					
			}
			if (dynamic_cast<PIPE*>(e->obj))
			{
				PIPE* pipe = dynamic_cast<PIPE*>(e->obj);
				if (pipe->GetType() == PIPE_TYPE_DOWN)
				{
					if ((pipe->x + PIPING_LIMIT_LEFT) <= this->x && this->x <= (pipe->x + PIPING_LIMIT_RIGHT))
					{
						this->canPipeDowning = true;
					}
					else
					{
						this->canPipeDowning = false;
					}
				}
				else if (pipe->GetType() == PIPE_TYPE_UP)
				{
					if ((pipe->x + PIPING_LIMIT_LEFT) <= this->x && this->x <= (pipe->x + PIPING_LIMIT_RIGHT))
					{
						this->canPipeUpping = true;
					}
					else
					{
						this->canPipeUpping = false;
					}
				}
				else
				{
					if (ny == 0 && nx != 0)
						control_able = 1 * nx;
				}
			}

			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario* mario_green = dynamic_cast<CMario*>(e->obj);
				if (this->type == MARIO_TYPE_RED && mario_green->type == MARIO_TYPE_GREEN)
				{

					this->SetState(MARIO_STATE_SITDOWN);
				}

			}

			if (dynamic_cast<CFlower*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						if (level == MARIO_LEVEL_TAIL)
						{
							level = MARIO_LEVEL_BIG;

						}
						else
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_STATE_DIE);
							else
								level = MARIO_LEVEL_SMALL;
					}

				}
			}
			else if (dynamic_cast<CFlowerBullet*>(e->obj))
			{
				if (untouchable == 0)
				{
					StartUntouchable();
					if (level == MARIO_LEVEL_TAIL)
					{
						level = MARIO_LEVEL_BIG;

					}
					else
						if (level == MARIO_LEVEL_SMALL)
							SetState(MARIO_STATE_DIE);
						else
							level = MARIO_LEVEL_SMALL;
				}
			}
			else if (dynamic_cast<CBell*>(e->obj))
			{
				if (e->ny < 0)
				{
					CBell* bell = dynamic_cast<CBell*>(e->obj);
					bell->SetState(BELL_STATE_DIE);
					for (UINT i = 0; i < coObjects->size(); i++)
					{
						LPGAMEOBJECT obj = coObjects->at(i);
						if (dynamic_cast<CBreakableBrick*>(obj))
						{
							CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*>(obj);
							if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
							{
								breakable_brick->SetState(BREAKABLE_BRICK_STATE_COIN);
							}
						}
					}
				}

			}
			else if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				if (e->ny > 0 || isSpining)
				{
					CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (question_brick->GetIsAlive())
					{
						question_brick->SetIsUp(true);
						question_brick->SetCalcYColli(true);
						question_brick->SetIsAlive(false);
						CGame::GetInstance()->MoneyUp();

					}
				}

			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (((nx != 0 && ny == 0) || (ny > 0 && nx == 0)) && breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
				{
					if (isSpining && breakable_brick->y >= this->y + 9)
					{
						breakable_brick->SetisBreaking(true);
						breakable_brick->BreakLocating();
						breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
					}
					else if (ny > 0 && nx == 0)
					{	
						breakable_brick->SetisBreaking(true);
						breakable_brick->BreakLocating();
						breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
					}
				}
				else if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_COIN)
				{
					CGame::GetInstance()->MoneyUp();
					breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
				}
			}
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (isSpining)
				{
					if (goomba->GetType() != GOOMBA_RED_FLY)
					{
						goomba->SetState(GOOMBA_STATE_DIE_2);
						goomba->SetSpeed(-nx * KOOPAS_DEFLECT_SPEED_X, KOOPAS_DEFLECT_SPEED_Y);
					}
					else
					{
						goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
						goomba->SetSpeed(-nx * KOOPAS_DEFLECT_SPEED_X, KOOPAS_DEFLECT_SPEED_Y);
					}
				}
				else
					if (e->ny < 0)
					{
						if (goomba->GetType() != GOOMBA_RED_FLY)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
						}
						else
						{
							goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							StartUntouchable();
							if (level == MARIO_LEVEL_TAIL)
							{
								level = MARIO_LEVEL_BIG;

							}
							else
								if (level == MARIO_LEVEL_SMALL)
									SetState(MARIO_STATE_DIE);
								else
									level = MARIO_LEVEL_SMALL;
						}
					}
			}
			else if (dynamic_cast<CSpecial_Item*>(e->obj))
			{
				StartSwitchScene();
				int time_picker = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetTimePicker();
				CSpecial_Item* special_item = dynamic_cast<CSpecial_Item*>(e->obj);
				int special_item_state = special_item->GetState();
				this->words_end_scene_item_id = special_item_state;
				switch (special_item_state)
				{
				case 100:
					special_item->SetState(400);
					break;
				case 200:
					special_item->SetState(500);
					break;
				case 300:
					special_item->SetState(600);
					break;
				}

				vector<int> items_render = CGame::GetInstance()->GetItemsTypeRender();
				for (int i = 0; i < items_render.size(); i++)
				{
					if (items_render[i] == 1)
					{
						switch (special_item_state)
						{
						case 100:
							items_render[i] = 3;
							break;
						case 200:
							items_render[i] = 2;
							break;
						case 300:
							items_render[i] = 4;
							break;
						}
						break;
					}
				}
				CGame::GetInstance()->SetItems(items_render);
				control_able = -2;
				StartSwitchScene();
				timetemp = time_picker;

			}
			else if (dynamic_cast<CKoopas*>(e->obj)) //  if KOOPAS
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELLING)
					{
						if (koopas->GetType() == KOOPAS_XANH_FLY)
						{
							koopas->SetType(KOOPAS_XANH_WALK);
							vy = MARIO_JUMP_DEFLECT_SPEED;
						}
						else
						{
							vy = MARIO_JUMP_DEFLECT_SPEED;
							koopas->SetState(KOOPAS_STATE_SHELLING);
							vx = 0;
						}
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELLING)
					{
						vy = MARIO_JUMP_DEFLECT_SPEED;
						if (this->nx > 0)
						{
							koopas->SetState(KOOPAS_STATE_SPINNING);
							koopas->nx = 1;
							koopas->vx = KOOPAS_SPINNING_SPEED;
						}
						else if (this->nx < 0)
						{
							koopas->SetState(KOOPAS_STATE_SPINNING);
							koopas->nx = -1;
							koopas->vx = -KOOPAS_SPINNING_SPEED;
						}
					}
				}
				else if (isSpining)
				{
					if (koopas->GetType() == KOOPAS_XANH_FLY)
					{
						koopas->SetType(KOOPAS_XANH_WALK);
					}
					koopas->SetShellUpRender(true);
					koopas->SetRenderRegconization(true);
					koopas->SetState(KOOPAS_STATE_SHELLING);
					koopas->SetSpeed(-nx * 0.15, -0.4);
				}
				else if (koopas->GetState() == KOOPAS_STATE_SHELLING)// koopas->GetisBeingHold() == 0)
				{
					if (ny == 0)
					{
						if (isHolding == 0)
						{
							isKicking = true;
							StartKicking();
							if (this->nx > 0)
							{
								koopas->SetState(KOOPAS_STATE_SPINNING);
								koopas->nx = 1;
								koopas->vx = KOOPAS_SPINNING_SPEED;
							}
							else if (this->nx < 0)
							{
								koopas->SetState(KOOPAS_STATE_SPINNING);
								koopas->nx = -1;
								koopas->vx = -KOOPAS_SPINNING_SPEED;
							}
						}
						koopas->SetisBeingHold(true);
					}
				}
				else
					if (e->nx != 0)
					{
						state = MARIO_STATE_IDLE;
						if (untouchable == 0)
						{
							if (isKicking == 0)
							{
								if (koopas->GetState() != KOOPAS_STATE_SHELLING)
								{
									if (untouchable == 0)
									{
										StartUntouchable();
										if (level == MARIO_LEVEL_TAIL)
										{
											level = MARIO_LEVEL_BIG;

										}
										else
											if (level == MARIO_LEVEL_SMALL)
												SetState(MARIO_STATE_DIE);
											else
												level = MARIO_LEVEL_SMALL;
									}
								}
							}
						}
					}
			}
		}
		if (level != ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetLvPre())
		{
			int lv = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetLvPre();
			switch (level)
			{
			case MARIO_LEVEL_BIG:
				if (lv == MARIO_LEVEL_TAIL)
					isSmokeTransform = true;
				else isTransform = true;
				break;
			case MARIO_LEVEL_TAIL:
				isSmokeTransform = true;
				break;
			default:
				isTransform = true;
				break;
			}
			StartIsTransform();
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	vector <LPCOLLISIONEVENT> collisionEvents;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CRECT*>(e->obj))
		{
			if (mario->dy < 0)// || mario->nx != 0)
				continue;
		}
		if (dynamic_cast<PIPE*>(e->obj))
		{
			if (mario->dy < 0)// || mario->nx != 0)
				continue;
		}
		if (!dynamic_cast<PIPE*>(e->obj))
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		}
		if (dynamic_cast<FIREBALL*>(e->obj))
		{
			continue;
		}
		if (dynamic_cast<CKoopas*>(e->obj))
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario->untouchable == 1)
				continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			collisionEvents.push_back(e);
		else
			delete e;
	}

	std::sort(collisionEvents.begin(), collisionEvents.end(), CCollisionEvent::compare);

	for (UINT i = 0; i < collisionEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(collisionEvents[i]->obj);
		if (e->t > 0 && e->t <= 1.0f)
		{
			/*float rmt = 1.0f - e->t;
			float DP = (vx * e->ny + vy * e->nx) * rmt;
			vx = DP * e->ny;
			vy = DP * e->nx * 2;*/
			coEvents.push_back(e);
		}
		else
			delete e;
	}
}
void CMario::Render()
{
	int ani = -1;
	if (isAppear)
	{
		switch (type)
		{
		case MARIO_TYPE_RED:

				if (isTransform)
					if (nx < 0) ani = MARIO_TRANSFORM_LEFT;
					else
						ani = MARIO_TRANSFORM_RIGHT;
				else
					if (isSmokeTransform)
						if (nx < 0) ani = MARIO_SMOKE_TRANSFORM_LEFT;
						else
							ani = MARIO_SMOKE_TRANSFORM_RIGHT;
					else if (level == MARIO_LEVEL_BIG)
					{
						if (isSitting)
						{
							if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
							else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
						}
						else if (state == MARIO_STATE_SPEED_DOWN)
							if (nx > 0)
								ani = MARIO_ANI_BIG_WALKING_RIGHT;
							else
								ani = MARIO_ANI_BIG_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_IDLE)
						{
							if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
							else ani = MARIO_ANI_BIG_IDLE_LEFT;
						}
						else if (GetisBraking())
						{
							if (nx > 0)
								ani = MARIO_ANI_BIG_BRAKING_RIGHT;
							else
								ani = MARIO_ANI_BIG_BRAKING_LEFT;
						}
						else if (GetState() == MARIO_STATE_WALKING_LEFT)
							ani = MARIO_ANI_BIG_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_WALKING_RIGHT)
							ani = MARIO_ANI_BIG_WALKING_RIGHT;
						else if (GetState() == MARIO_STATE_RUNNING_LEFT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_BIG_MAX_SPEED_LEFT;
							else ani = MARIO_ANI_BIG_RUNNING_LEFT;
						else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_BIG_MAX_SPEED_RIGHT;
							else ani = MARIO_ANI_BIG_RUNNING_RIGHT;
						else if (GetState() == MARIO_STATE_FLYING_LEFT)
							ani = MARIO_ANI_BIG_FLYING_LEFT;
						else if (GetState() == MARIO_STATE_FLYING_RIGHT)
							ani = MARIO_ANI_BIG_FLYING_RIGHT;
						if (GetIsJumping() == true)
							if (vy >= 0)
							{
								if (nx > 0) ani = MARIO_ANI_BIG_JUMP_FALLING_RIGHT;
								else ani = MARIO_ANI_BIG_JUMP_FALLING_LEFT;
							}
							else
							{
								if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
								else ani = MARIO_ANI_BIG_JUMPING_LEFT;
							}
						if (renderHolding)
						{
							if (vx != 0)
							{
								if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_RIGHT;
								else ani = MARIO_ANI_BIG_HOLDING_LEFT;
							}
							else
								if (nx > 0) ani = MARIO_ANI_BIG_IDLE_HOLDING_RIGHT;
								else ani = MARIO_ANI_BIG_IDLE_HOLDING_LEFT;
						}
						if (GetIsKicking() == true)
						{
							if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
							else ani = MARIO_ANI_BIG_KICKING_LEFT;
						}
					}

			//SMALL MARIO

					else if (level == MARIO_LEVEL_SMALL)
					{
						if (isSitting)
						{
							if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
							else ani = MARIO_ANI_SMALL_IDLE_LEFT;
						}
						else if (state == MARIO_STATE_SPEED_DOWN)
							if (nx > 0)
								ani = MARIO_ANI_SMALL_WALKING_RIGHT;
							else
								ani = MARIO_ANI_SMALL_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_IDLE)
						{
							if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
							else ani = MARIO_ANI_SMALL_IDLE_LEFT;
						}
						else if (GetisBraking())
						{
							if (nx > 0)
								ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
							else
								ani = MARIO_ANI_SMALL_BRAKING_LEFT;
						}
						else if (GetState() == MARIO_STATE_WALKING_LEFT)
							ani = MARIO_ANI_SMALL_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_WALKING_RIGHT)
							ani = MARIO_ANI_SMALL_WALKING_RIGHT;
						else if (GetState() == MARIO_STATE_RUNNING_LEFT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_SMALL_MAX_SPEED_LEFT;
							else ani = MARIO_ANI_SMALL_RUNNING_LEFT;
						else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_SMALL_MAX_SPEED_RIGHT;
							else ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
						else if (GetState() == MARIO_STATE_FLYING_LEFT)
							ani = MARIO_ANI_SMALL_FLYING_LEFT;
						else if (GetState() == MARIO_STATE_FLYING_RIGHT)
							ani = MARIO_ANI_SMALL_FLYING_RIGHT;
						//else if (isSitting)
						//{
						//	if (nx > 0) ani = MARIO_ANI_SMALL_SITDOWN_RIGHT;
						//	else ani = MARIO_ANI_SMALL_SITDOWN_LEFT;
						//} MARIO SMALL KO CO STATE NGOI
						if (state == MARIO_STATE_JUMP || GetIsJumping() == true)
							if (vy >= 0)
							{
								if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
								else ani = MARIO_ANI_SMALL_IDLE_LEFT;
							}
							else
							{
								if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
								else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
							}
						if (renderHolding)
						{
							if (vx != 0)
							{
								if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_RIGHT;
								else ani = MARIO_ANI_SMALL_HOLDING_LEFT;
							}
							else if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_HOLDING_RIGHT;
							else ani = MARIO_ANI_SMALL_IDLE_HOLDING_LEFT;
						}
						else if (GetIsKicking() == true)
						{
							if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
							else ani = MARIO_ANI_SMALL_KICKING_LEFT;
						}
						if (state == MARIO_STATE_DIE)
						{
							ani = MARIO_ANI_DIE;
						}
					}

			// TAIL MARIO

					else if (level == MARIO_LEVEL_TAIL)
					{
						if (isFlying)
						{
							if (nx > 0) ani = MARIO_ANI_TAIL_FLYING_RIGHT;
							else ani = MARIO_ANI_TAIL_FLYING_LEFT;
						}
						else if (isFalling)
						{
							if (nx > 0)
								ani = MARIO_ANI_FALLING_RIGHT;
							else
								ani = MARIO_ANI_FALLING_LEFT;
						}
						else if (GetIsKicking() == true)
						{
							if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
							else ani = MARIO_ANI_TAIL_KICKING_LEFT;
						}
						else if (GetIsJumping() == true)
							if (vy >= 0)
							{
								if (nx > 0) ani = MARIO_ANI_TAIL_JUMP_FALLING_RIGHT;
								else ani = MARIO_ANI_TAIL_JUMP_FALLING_LEFT;
							}
							else
							{
								if (nx > 0) ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
								else ani = MARIO_ANI_TAIL_JUMPING_LEFT;
							}
						else if (isSitting)
						{
							if (nx > 0) ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
							else ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
						}
						else if (GetState() == MARIO_STATE_IDLE)
						{
							if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
							else ani = MARIO_ANI_TAIL_IDLE_LEFT;
						}
						else if (GetisBraking())
						{
							if (nx > 0)
								ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
							else
								ani = MARIO_ANI_TAIL_BRAKING_LEFT;
						}
						else if (GetState() == MARIO_STATE_WALKING_LEFT)
							ani = MARIO_ANI_TAIL_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_WALKING_RIGHT)
							ani = MARIO_ANI_TAIL_WALKING_RIGHT;
						else if (GetState() == MARIO_STATE_RUNNING_LEFT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_TAIL_MAX_SPEED_LEFT;
							else ani = MARIO_ANI_TAIL_RUNNING_LEFT;
						else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_TAIL_MAX_SPEED_RIGHT;
							else ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
						else if (GetState() == MARIO_STATE_FLYING_LEFT)
							ani = MARIO_ANI_TAIL_FLYING_LEFT;
						else if (GetState() == MARIO_STATE_FLYING_RIGHT)
							ani = MARIO_ANI_TAIL_FLYING_RIGHT;
						else if (state == MARIO_STATE_SPEED_DOWN)
							if (nx > 0)
								ani = MARIO_ANI_TAIL_WALKING_RIGHT;
							else
								ani = MARIO_ANI_TAIL_WALKING_LEFT;
						if (GetIsSpining() == true)
						{
							if (nx > 1)
								ani = MARIO_ANI_TAIL_TURNING_RIGHT;
							else ani = MARIO_ANI_TAIL_TURNING_LEFT;
						}
						if (renderHolding)
						{
							if (vx != 0)
							{
								if (nx > 0) ani = MARIO_ANI_TAIL_HOLDING_RIGHT;
								else ani = MARIO_ANI_TAIL_HOLDING_LEFT;
							}
							else if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_HOLDING_RIGHT;
							else ani = MARIO_ANI_TAIL_IDLE_HOLDING_LEFT;
						}
					}

			// FIRE MARIO

					else if (level == MARIO_LEVEL_FIRE)
					{
						if (state == MARIO_STATE_SPEED_DOWN)
							if (nx > 0)
								ani = MARIO_ANI_FIRE_WALKING_RIGHT;
							else
								ani = MARIO_ANI_FIRE_WALKING_LEFT;
						else if (isSitting)
						{
							if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
							else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
						}
						else if (GetState() == MARIO_STATE_IDLE)
						{
							if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
							else ani = MARIO_ANI_FIRE_IDLE_LEFT;
						}
						else if (GetisBraking())
						{
							if (nx > 0)
								ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
							else
								ani = MARIO_ANI_FIRE_BRAKING_LEFT;
						}
						else if (GetState() == MARIO_STATE_WALKING_LEFT)
							ani = MARIO_ANI_FIRE_WALKING_LEFT;
						else if (GetState() == MARIO_STATE_WALKING_RIGHT)
							ani = MARIO_ANI_FIRE_WALKING_RIGHT;
						else if (GetState() == MARIO_STATE_RUNNING_LEFT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_FIRE_MAX_SPEED_LEFT;
							else ani = MARIO_ANI_FIRE_RUNNING_LEFT;
						else if (GetState() == MARIO_STATE_RUNNING_RIGHT)
							if (abs(vx) >= abs(MARIO_MAX_SPEED))
								ani = MARIO_ANI_FIRE_MAX_SPEED_RIGHT;
							else ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
						else if (GetState() == MARIO_STATE_FLYING_LEFT)
							ani = MARIO_ANI_FIRE_FLYING_LEFT;
						else if (GetState() == MARIO_STATE_FLYING_RIGHT)
							ani = MARIO_ANI_FIRE_FLYING_RIGHT;
						if (//GetState() == MARIO_STATE_JUMP)
							GetIsJumping() == true)
							if (vy >= 0)
							{
								if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_FALL_RIGHT;
								else ani = MARIO_ANI_FIRE_JUMP_FALL_LEFT;
							}
							else
							{
								if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
								else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
							}
						if (isFiring)
							if (nx > 0)
								ani = MARIO_ANI_SHOOTING_RIGHT;
							else
								ani = MARIO_ANI_SHOOTING_LEFT;
						else if (renderHolding)
						{
							if (vx != 0)
							{
								if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_RIGHT;
								else ani = MARIO_ANI_FIRE_HOLDING_LEFT;
							}
							else
								if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_HOLDING_RIGHT;
								else ani = MARIO_ANI_FIRE_IDLE_HOLDING_LEFT;
						}
						else if (GetIsKicking() == true)
						{
							if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
							else ani = MARIO_ANI_FIRE_KICKING_LEFT;
						}
					}
					if (state == MARIO_STATE_PIPE_DOWNING || state == MARIO_STATE_PIPE_UPPING)
					{
						if (level == MARIO_LEVEL_BIG)
						{
							ani = MARIO_ANI_BIG_PIPE;
						}
						else if (level == MARIO_LEVEL_SMALL)
						{
							ani = MARIO_ANI_SMALL_PIPE;
						}
						else if (level == MARIO_LEVEL_TAIL)
						{
							ani = MARIO_ANI_TAIL_PIPE;
						}
						else
						{
							ani = MARIO_ANI_FIRE_PIPE;
						}
					}
			break;


		case MARIO_TYPE_GREEN:

			if (isKicking)
			{
				if (nx > 0) ani = MARIO_GREEN_ANI_BIG_KICKING_RIGHT;
				else ani = MARIO_GREEN_ANI_BIG_KICKING_LEFT;

			}

			else if (renderHolding)
			{
				if (vx == 0)
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_HOLDING_IDLE_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_HOLDING_IDLE_LEFT;

				}
				else
				{

					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_HOLDING_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_HOLDING_LEFT;

				}
			}

			else if (state == MARIO_STATE_IDLE)
			{

				if (nx > 0) ani = MARIO_GREEN_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_GREEN_ANI_BIG_IDLE_LEFT;

			}



			else if (isJumping == true)
			{

				if (vy < 0)
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_JUMPING_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_JUMPING_LEFT;
				}
				else
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_JUMP_FALL_RIGHT;
					else ani = MARIO_GREEN_ANI_JUMP_FALL_LEFT;
				}

			}


			else if (state == MARIO_STATE_RUNNING_RIGHT)
			{
				ani = MARIO_GREEN_ANI_BIG_RUNNING_RIGHT;
			}

			else if (state == MARIO_STATE_RUNNING_LEFT)
			{
				ani = MARIO_GREEN_ANI_BIG_RUNNING_LEFT;

			}



			else if (nx > 0) // walking right
			{
				//if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_GREEN_ANI_BIG_WALKING_RIGHT;

			}

			else if (nx < 0) // walking left
			{

				ani = MARIO_GREEN_ANI_BIG_WALKING_LEFT;
			}
			break;
		}
	}
	if (ani == -1) return;

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
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
		/*case MARIO_STATE_ON_FIRE:
			SetLevel(MARIO_LEVEL_FIRE);
			break;*/
	case MARIO_STATE_SITDOWN:
		//vx = 0;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (BrakingCalculation() == false)
		{
			vx = MARIO_WALKING_SPEED;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		if (BrakingCalculation() == false)
		{
			vx = -MARIO_WALKING_SPEED;
		}
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		if (BrakingCalculation() == false)
		{
			if (vx >= MARIO_MAX_SPEED)
			{
				vx = MARIO_MAX_SPEED;
			}
			else vx = MARIO_WALKING_SPEED + MARIO_ACCELERATE_SPEED * speedLevel;
			if (isFlying)
			{
				vx = MARIO_WALKING_SPEED;
			}
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		nx = -1;
		if (BrakingCalculation() == false)
		{
			if (vx <= -MARIO_MAX_SPEED)
			{
				vx = -MARIO_MAX_SPEED;
			}
			else vx = -MARIO_WALKING_SPEED + -MARIO_ACCELERATE_SPEED * speedLevel;
			if (isFlying)
			{
				vx = -MARIO_WALKING_SPEED;
			}
		}
		break;
	case MARIO_STATE_FLYING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_FLYING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_FALLING_DOWN:
		vy = MARIO_FALLING_DOWN_SPEED;
		break;
	case MARIO_STATE_SPEED_DOWN:
		if (vx > 0)
		{
			vx -= MARIO_SPEED_DOWN;
		}
		else if (vx < 0)
		{
			vx += MARIO_SPEED_DOWN;
		}
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_JUMP_HIGH:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = MARIO_JUMP_HIGH_SPEED_Y;
		break;
	case MARIO_STATE_PIPE_DOWNING:
		vy = 0.01f;
		break;
	case MARIO_STATE_PIPE_UPPING:
		vy = -0.01f;
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
	if (state == MARIO_STATE_SITDOWN)
	{
		if (level != MARIO_LEVEL_SMALL)
			top = top + MARIO_SMALL_BBOX_HEIGHT_DIFF;
	}
	if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
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
	if (isTransform)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
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
