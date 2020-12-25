#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "Coin.h"
#include "define.h"


class CBreakableBrick : public CGameObject
{
	float breakX = 0;
	float breakY = 0;

	bool isBreaking = false;
	int count = 0;
public:
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
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CBreakableBrick();
	virtual void SetState(int state);


};