#pragma once
#include "GameObject.h"
#include "algorithm"

class CFlower : public CGameObject
{
	int type;
	int isShootingUp = 1;
	DWORD time_showing = 0;
	bool isUp = true;
	bool isFiring = false;
	bool isFired = false;
	bool isAlive = true;

	bool isAllowFlowerToUpdate = true;


	bool isAllowToShowScore = false;
	DWORD timing_score;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CFlower(int ctype);
	virtual void SetState(int state);
	void StartShowing()
	{
		time_showing = GetTickCount();
	}
	int GetIsShootingUp()
	{
		return isShootingUp;
	}
	void SetIsShootingUp(int isShootingUpInt)
	{
		isShootingUp = isShootingUpInt;
	}
	bool GetIsFiring()
	{
		return isFiring;
	}
	void SetIsFiring(bool isFiringBool)
	{
		isFiring = isFiringBool;
	}
	bool GetIsFired()
	{
		return isFired;
	}
	void SetIsFired(bool isFiredBool)
	{
		isFired = isFiredBool;
	}
	bool GetIsAlive()
	{
		return isAlive;
	}
	void SetIsAlive(bool isAliveBool)
	{
		isAlive = isAliveBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	bool GetIsAllowToShowScore()
	{
		if (this != NULL)
			return isAllowToShowScore;
		else
		{
			return false;
		}
	}
	void SetIsAllowToShowScore(bool isAllowToShowScoreBool)
	{
		if (this != NULL)
			isAllowToShowScore = isAllowToShowScoreBool;
	}
	void StartTimingScore()
	{
		timing_score = GetTickCount();
	}
	bool GetIsAllowFlowerToUpdate()
	{
		return isAllowFlowerToUpdate;
	}
	void SetIsAllowFlowerToUpdate(bool isAllowFlowerToUpdateBool)
	{
		isAllowFlowerToUpdate = isAllowFlowerToUpdateBool;
	}
};