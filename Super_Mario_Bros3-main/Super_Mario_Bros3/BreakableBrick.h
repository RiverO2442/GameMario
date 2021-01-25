#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "Coin.h"
#include "define.h"

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
	void SetbreakY(float value)
	{
		breakY = value;
	}
	float GetbreakY()
	{
		return breakY;
	}
	void SetbreakX(float value)
	{
		breakX = value;
	}
	float GetbreakX()
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
		reviveTime = (DWORD)GetTickCount64();
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