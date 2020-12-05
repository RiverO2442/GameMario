#pragma once
#include "GameObject.h"
#include "algorithm"
#include "define.h"

class CFlower : public CGameObject
{
	int type;
	int isShootingUp = 1;
	DWORD time_showing = 0;
	bool isUp = true;
	bool isFiring = false;
	bool isFired = false;

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
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
};