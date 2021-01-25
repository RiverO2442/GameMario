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
	level = CGame::GetInstance()->GetMarioLevelWorldMap();
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
			if (isJumping)
			{
				nx = 0;
				ny = 0;
			}
			else
				ny = -MARIO_MOVINGHORIZONTALRECTANGLE_PUSH_UP;
		}


		if (dynamic_cast<CLeaf*>(c->obj) || dynamic_cast<CMushRoom*>(c->obj) || dynamic_cast<CFlowerBullet*>(c->obj) || dynamic_cast<CKoopas*>(c->obj) || dynamic_cast<CGoomba*>(c->obj) || dynamic_cast<CMario*>(c->obj))
		{
			if (ny != 0)
				ny = -MARIO_MUSHROOM_PUSH_UP;
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

		if (dynamic_cast<CMovingHorizontalRectangle*>(c->obj))
		{
			ny = -MARIO_MOVINGHORIZONTALRECTANGLE_PUSH_UP;
		}
		//if (dynamic_cast<CRECT*>(c->obj))
		//{
		//	if (ny > 0)//|| nx != 0)
		//	{
		//		nx = 0;
		//		ny = 0;
		//	}
		//}
	}//

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CMario::TimingAndStateBasedEventCal(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Simple fall down
	if (state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING && !isTransform && !isSmokeTransform)
		vy += MARIO_GRAVITY * dt;
	if (isTransform || isSmokeTransform)
	{
		vx = 0;
		vy = 0;
	}
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id != INTRO_SCENE_ID)
	{
		if (jumping_long_start != 0)
		{
			if ((DWORD)GetTickCount64() - jumping_long_start >= MARIO_JUMP_LONG_START_TIME)
			{
				sub_jump = true;
				Start_jumping_long();
				mario_time_jump++;
			}
		}

		if (sub_jump)
			if (main_jump)
			{
				sub_jump = false;
				SetState(MARIO_STATE_JUMP);
				if (mario_time_jump >= MARIO_MAX_LONG_JUMP_TIME)
				{
					Setcanjumplong(false);
					mario_time_jump = 0;
					jumping_long_start = 0;
					main_jump = false;
				}
			}

		if (keep_stack_start != 0)
			if ((DWORD)GetTickCount64() - keep_stack_start > MARIO_KEEP_STACK_TIME)
			{
				isKeepStack = false;
				speedLevel = 0;
				keep_stack_start = 0;
			}
		if (state == MARIO_STATE_DIE && GetcanSetLifeDown())
		{
			StartSwitchScene();
			CGame::GetInstance()->SetLifeDown();
			SetcanSetLifeDown(false);
			SetControl(LOSE_ALL_CONTROL);
		}

		if (switch_scene_start != 0)
			if (state == MARIO_STATE_DIE && (DWORD)GetTickCount64() - switch_scene_start >= MARIO_DIE_TIME_LIMIT)
			{
				CGame::GetInstance()->SetControlMarioRenderWorldMap(true);
				CGame::GetInstance()->SetSavedNodeID(0);
				CGame::GetInstance()->SwitchScene(2);
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
			SetControl(LOSE_ALL_CONTROL);
			if ((DWORD)GetTickCount64() - switch_scene_start >= MARIO_WORLD_END_SCENE_SHOW_TIME)
			{
				this->isAllowToShowWordsEndScene = true;
			}
			if ((DWORD)GetTickCount64() - switch_scene_start >= MARIO_TIME_TO_POINT_TIME)
			{
				StartCountDownTimePicker();
				if ((DWORD)GetTickCount64() - count_down_time_start >= MARIO_TIME_PICKER_SUBBING_TIME)
				{
					if (time_picker > 0)
					{
						((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SetTimeDown();
						count_down_time_start = 0;
					}
				}

			}
			if ((DWORD)GetTickCount64() - switch_scene_start >= MARIO_CAL_TOTAL_POINT_TIME)
			{
				if (time_picker == 0 && timetemp != 0)
				{
					CGame::GetInstance()->ScoreUp(TIME_VALUE * timetemp);
					timetemp = 0;
				}
			}

			if ((DWORD)GetTickCount64() - switch_scene_start >= MARIO_SWITCH_SCENE_TIME)
			{
				this->isAllowToRenderItemAnimation = false;
				CGame::GetInstance()->SetMarioLevelWorldMap(this->level);
				if (id == SCENE_1_1_ID)
				{
					CGame::GetInstance()->SetIsPassedScene1_1(true);
					CGame::GetInstance()->SetControlMarioRenderWorldMap(true);
				}
				else if (id == SCENE_1_4_ID)
				{
					CGame::GetInstance()->SetIsPassedScene1_4(true);
					CGame::GetInstance()->SetControlMarioRenderWorldMap(true);
				}
				switch_scene_start = 0;
				SetControl(0);
				CGame::GetInstance()->SwitchScene(WORLD_MAP_ID);
				return;
			}

			DebugOut1(L"[INFO] level La : %d \n", (DWORD)GetTickCount64() - switch_scene_start);
		}

		if (state == MARIO_STATE_PIPE_DOWNING)
		{
			if ((DWORD)GetTickCount64() - pipe_downing_start >= MARIO_PIPING_DOWN_TIME_LIMIT)
			{
				isHolding = false;
				renderHolding = false;
			}

			if ((DWORD)GetTickCount64() - pipe_downing_start >= MARIO_PIPING_DOWN_TIME_LIMIT_2)
			{
				canPipeDowning = false;
				SetState(MARIO_STATE_IDLE);
				pipe_downing_start = 0;
			}


			if (!isKeepStack)
				DEACCELERETING();
		}

		if (state == MARIO_STATE_PIPE_UPPING)
		{
			if (id == 3)
			{
				if ((DWORD)GetTickCount64() - pipe_upping_start >= MARIO_PIPE_UPPING_TIME_LIMIT)
				{
					isAtTheTunnel = false;
				}


				if (this->level != MARIO_LEVEL_SMALL)
				{
					if ((DWORD)GetTickCount64() - pipe_upping_start >= MARIO_PIPE_UPPING_TIME_LIMIT_2)
					{
						canPipeUpping = false;
						SetState(MARIO_STATE_IDLE);
						pipe_upping_start = 0;
					}
				}
				else
				{
					if ((DWORD)GetTickCount64() - pipe_upping_start >= MARIO_PIPE_UPPING_TIME_LIMIT_2_SMALL)
					{
						canPipeUpping = false;
						SetState(MARIO_STATE_IDLE);
						pipe_upping_start = 0;
					}
				}
			}
			else if (id == 4)
			{
				if (this->level != MARIO_LEVEL_SMALL)
				{
					if ((DWORD)GetTickCount64() - pipe_upping_start >= MARIO_PIPE_UPPING_TIME_LIMIT_2_1_4)
					{
						canPipeUpping = false;
						SetState(MARIO_STATE_IDLE);
						pipe_upping_start = 0;
					}
				}
				else
				{
					if ((DWORD)GetTickCount64() - pipe_upping_start >= MARIO_PIPE_UPPING_TIME_LIMIT_2_1_4_SMALL)
					{
						canPipeUpping = false;
						SetState(MARIO_STATE_IDLE);
						pipe_upping_start = 0;
					}
				}

			}

			if (!isKeepStack)
				DEACCELERETING();
		}
		// reset untouchable timer if untouchable time has passed
		if ((DWORD)GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}

		if ((DWORD)GetTickCount64() - firing_start > MARIO_FIRING_TIME)
		{
			isFiring = 0;
			firing_start = 0;
		}

		if ((DWORD)GetTickCount64() - transform_start > MARIO_TRANSFORM_TIME)
		{
			transform_start = 0;
			isTransform = false;
			isSmokeTransform = false;
		}

		if ((DWORD)GetTickCount64() - kicking_start > MARIO_KICKING_TIME)
		{
			SetIsKicking(false);
		}


		if ((DWORD)GetTickCount64() - spining_start > MARIO_SPINING_TIME)
		{
			SetIsSpining(false);
			spining_start = 0;
		}


		if (GetspeedLevel() < MARIO_MAX_SPEED_LEVEL)
		{
			canFly = false;
			isFlying = false;
		}

		if (abs((y - heightLimit)) >= 1)
		{
			if (!isOnMovingHorizontalRectangle)
				isJumping = true;
		}
	}
}

bool CMario::HitByTail(LPGAMEOBJECT obj, int state, bool Impacted)
{
	if (this->GetIsSpining())
	{
		float leftRec = 0, rightRec = 0, topRec = 0, bottomRec = 0;

		int Direction = 0;


		if ((DWORD)GetTickCount64() - this->Getspining_start() <= MARIO_SPINNING_TIME_LIMIT)
		{
			if (this->nx > 0)
			{
				leftRec = this->x - MARIO_LEFT_RECT_LEGHT;
				topRec = this->y + MARIO_TOP_RECT_LEGHT;
				rightRec = this->x;
				bottomRec = topRec + MARIO_BOTTOM_RECT_LEGHT;
				Direction = -1;
			}
			else
			{
				leftRec = this->x;
				topRec = this->y + MARIO_TOP_RECT_LEGHT;
				rightRec = leftRec + MARIO_RIGHT_RECT_LEGHT;
				bottomRec = topRec + MARIO_BOTTOM_RECT_LEGHT;
				Direction = 1;

			}
		}
		else
		{
			if (this->nx > 0)
			{
				leftRec = this->x;
				topRec = this->y + MARIO_TOP_RECT_LEGHT;
				rightRec = leftRec + MARIO_RIGHT_RECT_LEGHT;
				bottomRec = topRec + MARIO_BOTTOM_RECT_LEGHT;
				Direction = 1;
			}
			else
			{
				leftRec = this->x - MARIO_LEFT_RECT_LEGHT;
				topRec = this->y + MARIO_TOP_RECT_LEGHT;
				rightRec = this->x;
				bottomRec = topRec + MARIO_BOTTOM_RECT_LEGHT;
				Direction = -1;
			}
		}


		float leftRecobj, rightRecobj, topRecobj, bottomRecobj;

		obj->GetBoundingBox(leftRecobj, topRecobj, rightRecobj, bottomRecobj);

		if (((leftRec <= rightRecobj && leftRec >= leftRecobj) || (rightRec <= rightRecobj && rightRec >= leftRecobj) || ((leftRec <= leftRecobj) && (rightRec >= rightRecobj))) && ((topRec <= bottomRecobj && topRec >= topRecobj) || (bottomRec <= bottomRecobj && bottomRec >= topRecobj)))
		{
			if (obj->GetState() != state)
				obj->SetState(state);
			if (Impacted)
			{
				obj->SetSpeed(Direction * DIE_DEFLECT_SPEED_X, DIE_DEFLECT_SPEED_Y);
			}
			return true;
		}
	}

	return false;

}

void CMario::coEventCal(vector<LPGAMEOBJECT>* coObjects)
{
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy 
	CGameObject::Update(dt);

	TimingAndStateBasedEventCal(dt, coObjects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isFlying)
	{
		isKeepStack = true;
		Startkeep_stack();
	}

	if (state != MARIO_STATE_DIE && state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING)
		CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		heightLimit = (int)y;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		CPlayScene* playscene = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene());

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		if (ny < 0)    // Handle Jumping
		{
			if (vx == 0)
			{
				control_able = 0;
			}

			isJumping = false;
			isFalling = false;
			canFall = false;
			canjumplong = true;
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (ny < 0)
			{
				Setcanjumplong(true);
				SetIsJumping(0);
				SetCanFly(0);
			}

			if (ny > 0 && nx == 0)
			{
				Setcanjumplong(false);
				mario_time_jump = 0;
				jumping_long_start = 0;
				main_jump = false;
			}

			if (dynamic_cast<CCoin*>(e->obj)) // if e->obj is Coin
			{
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (coin->GetType() == COIN_NORMAL)
				{
					CGame::GetInstance()->ScoreUp(50);
					coin->SetIsAppear(false);
					CGame::GetInstance()->MoneyUp();

				}

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
							koopas->nx = RIGHT_DIRECTIOM;
							koopas->vx = KOOPAS_SPINNING_SPEED;
						}
						else if (this->nx < 0)
						{
							koopas->SetState(KOOPAS_STATE_SPINNING);
							koopas->nx = LEFT_DIRECTIOM;
							koopas->vx = -KOOPAS_SPINNING_SPEED;
						}
					}
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
								koopas->nx = RIGHT_DIRECTIOM;
								koopas->vx = KOOPAS_SPINNING_SPEED;
							}
							else if (this->nx < 0)
							{
								koopas->SetState(KOOPAS_STATE_SPINNING);
								koopas->nx = LEFT_DIRECTIOM;
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
					/*if (ny == 0 && nx != 0)
						control_able = 1 * nx;*/
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
			else if (dynamic_cast<CBoomerang*>(e->obj))
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
			else if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				CQuestionBrick* question_brick = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->ny > 0)
				{
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id == 3)
					{
						if (question_brick->GetIsAlive())
						{
							if (!question_brick->GetIsAllowQuestionBrickSlide())
							{
								question_brick->SetIsUp(true);
								question_brick->SetIsAlive(false);
								MushroomCheckPosition = this->x;
								question_brick->SetIsAllowToShowScore(true);
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
									MushroomCheckPosition = this->x;
									question_brick->SetIsAllowToShowScore(true);
									question_brick->SetIsAllowQuestionBrickSlide(true);
								}
							}
						}
					}
				}


			}
			else if (dynamic_cast<CBoomerangEnemy*>(e->obj))
			{
				CBoomerangEnemy* boomerang_enemy = dynamic_cast<CBoomerangEnemy*>(e->obj);
				if (e->ny < 0)
				{
					if (boomerang_enemy->GetIsAlive())
					{
						boomerang_enemy->SetIsAlive(false);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
						CGame::GetInstance()->ScoreUp(MERANG_VALUE);
						playscene->AddScore(x, y + SCORE_FIX_PST_Y, MERANG_VALUE);
					}
				}
				else
				{
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							StartIsTransform();
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);

					}
				}
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				CBreakableBrick* breakable_brick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (((nx != 0 && e->ny == 0) || (e->ny > 0 && nx == 0)) && breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
				{
					if (e->ny > 0 && nx == 0)
					{
						if (level != MARIO_LEVEL_SMALL)
						{
							breakable_brick->SetisBreaking(true);
							breakable_brick->BreakLocating();
							breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
						}
						else
						{
							breakable_brick->SetState(BREAKABLE_BRICK_STATE_SLIDING);
							breakable_brick->SetIsUp(true);
							if (!breakable_brick->GetIsAllowQuestionBrickSlide())
								breakable_brick->SetIsAllowQuestionBrickSlide(true);
						}

					}
				}
				else if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_COIN)
				{
					CGame::GetInstance()->MoneyUp();
					breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
				}
			}
			else if (dynamic_cast<CMovingHorizontalRectangle*>(e->obj))
			{
				CMovingHorizontalRectangle* moving_horizontal_rectangle = dynamic_cast<CMovingHorizontalRectangle*>(e->obj);
				if (e->ny < 0 && nx == 0)
				{
					isOnMovingHorizontalRectangle = true;
					this->mario_current_moving_horizontal_rec_id = moving_horizontal_rectangle->GetId();
					if (moving_horizontal_rectangle->GetState() == MOVING_HORIZONTAL_RECTANGLE_STATE_NORMAL)
					{
						moving_horizontal_rectangle->SetState(MOVING_HORIZONTAL_RECTANGLE_STATE_DOWN);
					}
					controlMarioColliWithMovingRec = false;
				}
				else if (nx != 0 && ny == 0)
				{
					controlMarioColliWithMovingRec = true;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
						if (goomba->GetType() != GOOMBA_RED_FLY)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, GOOMBA_VALUE);
							SetSpeed(nx * DIE_DEFLECT_SPEED_X, DIE_DEFLECT_SPEED_Y);
						}
						else if (goomba->GetState() != GOOMBA_STATE_RED_LOSE_WINGS)
						{
							goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
							playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, GOOMBA_VALUE);
							SetSpeed(nx * DIE_DEFLECT_SPEED_X, DIE_DEFLECT_SPEED_Y);
						}
						else
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								goomba->SetState(GOOMBA_STATE_DIE);
								playscene->AddScore(this->x, this->y + SCORE_FIX_PST_Y, GOOMBA_VALUE);
								SetSpeed(nx * DIE_DEFLECT_SPEED_X, DIE_DEFLECT_SPEED_Y);
							}
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
				case SPECIAL_ITEM_STATE_FLOWER_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_FLOWER_UP);
					break;
				case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_MUSHROOM_UP);
					break;
				case SPECIAL_ITEM_STATE_STAR_IDLE:
					special_item->SetState(SPECIAL_ITEM_STATE_STAR_UP);
					break;
				}

				vector<int> items_render = CGame::GetInstance()->GetItemsTypeRender();
				for (unsigned int i = 0; i < items_render.size(); i++)
				{
					if (items_render[i] == 1)
					{
						switch (special_item_state)
						{
						case SPECIAL_ITEM_STATE_FLOWER_IDLE:
							items_render[i] = HUD_TYPE_ITEM_ANI_FLOWER;
							break;
						case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
							items_render[i] = HUD_TYPE_ITEM_ANI_MUSHROOM;
							break;
						case SPECIAL_ITEM_STATE_STAR_IDLE:
							items_render[i] = HUD_TYPE_ITEM_ANI_STAR;
							break;
						}
						break;
					}
				}
				CGame::GetInstance()->SetItems(items_render);
				control_able = CONTROL_LOSE_ALL;
				StartSwitchScene();
				timetemp = time_picker;

			}
			if (!dynamic_cast<CMovingHorizontalRectangle*>(e->obj))
			{
				isOnMovingHorizontalRectangle = false;
				mario_current_moving_horizontal_rec_id = -1;
				controlMarioColliWithMovingRec = false;
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
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();

	if (id == SCENE_1_4_ID)
	{
		if (this->x <= CGame::GetInstance()->GetCamX() + MARIO_CAM_MOVE_PUSH_X_LEFT)
		{
			x = CGame::GetInstance()->GetCamX() + MARIO_CAM_MOVE_PUSH_X_LEFT;
			this->isCamPushRender = true;
		}
		else
		{
			this->isCamPushRender = false;
		}
		if (x >= CGame::GetInstance()->GetCamX() + CGame::GetInstance()->GetScreenWidth() - MARIO_BIG_BBOX_WIDTH)
		{
			x = (float)(CGame::GetInstance()->GetCamX() + CGame::GetInstance()->GetScreenWidth() - MARIO_BIG_BBOX_WIDTH);
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
			if (mario->vy < 0)// || mario->nx != 0)
				continue;
		}
		if (dynamic_cast<CPortal*>(e->obj))
		{
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
			if (isCamPushRender)
			{
				if (nx > 0)
				{
					if (level == MARIO_LEVEL_SMALL)
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					if (level == MARIO_LEVEL_BIG)
						ani = MARIO_ANI_BIG_WALKING_RIGHT;
					if (level == MARIO_LEVEL_TAIL)
						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
					if (level == MARIO_LEVEL_FIRE)
						ani = MARIO_ANI_FIRE_WALKING_RIGHT;
				}
				else
				{
					if (level == MARIO_LEVEL_SMALL)
						ani = MARIO_ANI_SMALL_WALKING_LEFT;
					if (level == MARIO_LEVEL_BIG)
						ani = MARIO_ANI_BIG_WALKING_LEFT;
					if (level == MARIO_LEVEL_TAIL)
						ani = MARIO_ANI_TAIL_WALKING_LEFT;
					if (level == MARIO_LEVEL_FIRE)
						ani = MARIO_ANI_FIRE_WALKING_LEFT;
				}
			}
			else if (isTransform)
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
							if (nx > 0)
								if (speedLevel == MARIO_MAX_SPEED_LEVEL)
									ani = MARIO_ANI_BIG_JUMP_MAX_POWER_RIGHT;
								else
									ani = MARIO_ANI_BIG_JUMPING_RIGHT;
							else if (speedLevel == MARIO_MAX_SPEED_LEVEL)
								ani = MARIO_ANI_BIG_JUMP_MAX_POWER_LEFT;
							else
								ani = MARIO_ANI_BIG_JUMPING_LEFT;
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
							if (nx > 0)
								if (speedLevel == MARIO_MAX_SPEED_LEVEL)
									ani = MARIO_ANI_SMALL_JUMP_MAX_POWER_RIGHT;
								else
									ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
							else if (speedLevel == MARIO_MAX_SPEED_LEVEL)
								ani = MARIO_ANI_SMALL_JUMP_MAX_POWER_LEFT;
							else
								ani = MARIO_ANI_SMALL_JUMPING_LEFT;
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
							if (nx > 0)
								if (speedLevel == MARIO_MAX_SPEED_LEVEL)
									ani = MARIO_ANI_TAIL_JUMP_MAX_POWER_RIGHT;
								else
									ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
							else if (speedLevel == MARIO_MAX_SPEED_LEVEL)
								ani = MARIO_ANI_TAIL_JUMP_MAX_POWER_LEFT;
							else
								ani = MARIO_ANI_TAIL_JUMPING_LEFT;
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
							if (nx > 0)
								if (speedLevel == MARIO_MAX_SPEED_LEVEL)
									ani = MARIO_ANI_FIRE_JUMP_MAX_POWER_RIGHT;
								else
									ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
							else if (speedLevel == MARIO_MAX_SPEED_LEVEL)
								ani = MARIO_ANI_FIRE_JUMP_MAX_POWER_LEFT;
							else
								ani = MARIO_ANI_FIRE_JUMPING_LEFT;
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
		nx = RIGHT_DIRECTIOM;
		if (BrakingCalculation() == false)
		{
			vx = MARIO_WALKING_SPEED;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = LEFT_DIRECTIOM;
		if (BrakingCalculation() == false)
		{
			vx = -MARIO_WALKING_SPEED;
		}
		break;
	case MARIO_STATE_JUMPING_LONGER:
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = RIGHT_DIRECTIOM;
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
		nx = LEFT_DIRECTIOM;
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
		nx = RIGHT_DIRECTIOM;
		break;
	case MARIO_STATE_FLYING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = LEFT_DIRECTIOM;
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
		vx = MARIO_MARIO_STATE_IDLE_SPEED;
		vy = MARIO_PIPING_SPEED;
		break;
	case MARIO_STATE_PIPE_UPPING:
		vy = -MARIO_PIPING_SPEED;
		vx = MARIO_MARIO_STATE_IDLE_SPEED;
		break;
	case MARIO_STATE_DIE:
		vx = MARIO_MARIO_STATE_IDLE_SPEED;
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
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (isTransform)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
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
	SetSpeed(MARIO_MARIO_STATE_IDLE_SPEED, MARIO_MARIO_STATE_IDLE_SPEED);
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
