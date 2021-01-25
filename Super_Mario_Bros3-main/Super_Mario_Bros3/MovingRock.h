#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"



#define MOVING_ROCK_STATE_IDLE	0
#define MOVING_ROCK_STATE_MOVE	100
#define	MOVING_ROCK_STATE_TOP_RIGHT	1
#define	MOVING_ROCK_STATE_TOP_LEFT	2
#define	MOVING_ROCK_STATE_BOTTOM_RIGHT	3
#define	MOVING_ROCK_STATE_BOTTOM_LEFT	4
#define MOVING_ROCK_GRAVITY		0.0015f





class CMovingRock : public CGameObject
{
	int type;
	bool isUsed = false;
	DWORD timing_start = 0;
public:

	CMovingRock();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = (DWORD)GetTickCount64();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
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