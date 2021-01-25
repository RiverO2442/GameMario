#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "define.h"

class CCoin : public CGameObject
{
	bool isAppear;
	int type;
	DWORD timing_start;
public:

	CCoin(int ctype);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void SetState(int state);
	void StartTiming()
	{
		timing_start = (DWORD)GetTickCount64();
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
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