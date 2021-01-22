#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "Coin.h"

#define BREAKABLE_BRICK_BBOX_WIDTH		16
#define BREAKABLE_BRICK_BBOX_HEIGHT		16

#define BREAKABLE_BRICK_STATE_NORMAL	0
#define BREAKABLE_BRICK_STATE_BREAK		100
#define BREAKABLE_BRICK_STATE_COIN		200
#define BREAKABLE_BRICK_STATE_SLIDING	300



#define BREAKABLE_BRICK_ANI_NORMAL		0
#define BREAKABLE_BRICK_ANI_COIN		1


class CBreakableBrick : public CGameObject
{
	DWORD reviveTime = 0;
	bool isRevive = false;
	float breakX = 0;
	float breakY = 0;

	bool isBreaking = false;
	int count = 0;
	bool isUp = false;
	int time_Y_Up = 0;
	bool isAllowQuestionBrickSlide = false;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CBreakableBrick();
	void BreakLocating()
	{
		GetPosition(breakX, breakY);
	}
	void SetbreakY(int value)
	{
		breakY = value;
	}
	int GetbreakY()
	{
		return breakY;
	}
	void SetbreakX(int value)
	{
		breakX = value;
	}
	int GetbreakX()
	{
		return breakX;
	}
	void Setcountup(int value = 1)
	{
		count = count + value;
	}
	int Getcount()
	{
		return count;
	}
	void Setcount(int value)
	{
		count = value;
	}
	bool GetisBreaking()
	{
		return isBreaking;
	}
	void SetisBreaking(bool value)
	{
		isBreaking = value;
	}
	void StartRevive()
	{
		reviveTime = GetTickCount();
	}
	virtual void SetState(int state);
	bool GetIsRevive()
	{
		return isRevive;
	}
	void SetIsRevive(bool IsReviveBool)
	{
		isRevive = IsReviveBool;
	}
	bool GetIsUp()
	{
		return isUp;
	}
	void SetIsUp(bool isUpBool)
	{
		isUp = isUpBool;
	}
	bool GetIsAllowQuestionBrickSlide()
	{
		return isAllowQuestionBrickSlide;
	}
	void SetIsAllowQuestionBrickSlide(bool isAllowQuestionBrickSlideBool)
	{
		isAllowQuestionBrickSlide = isAllowQuestionBrickSlideBool;
	}
};