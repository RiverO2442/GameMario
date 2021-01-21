#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Koopas.h"

class CGoomba : public CGameObject
{
	int type;
	bool isAppear = true;
	DWORD  jumpingStart = 0;
	DWORD  dyingStart = 0;


	DWORD  runningStart = 0;

	DWORD  time_switch_state = 0;

	int control_jump_time = 0;

	bool control_flying = false;

	int leftRec, rightRec, topRec, bottomRec;


	int pointPara = 1;


	bool isAllowToShowScore = false;
	DWORD timing_score;
public:
	CGoomba(int ctype, int scene_id);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	int GetType()
	{
		return type;
	}
	virtual void SetState(int state);
	void StartDying()
	{
		dyingStart = GetTickCount();
	}
	void StartRunning()
	{
		runningStart = GetTickCount();
	}
	int GetPointPara()
	{
		return pointPara;
	}
	void CalcDoublePointPara()
	{
		pointPara *= 2;
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
	void StartTimeSwitchingState()
	{
		if (time_switch_state == 0)
		{
			time_switch_state = GetTickCount();
		}
	}
	void StartJumping()
	{
		if (jumpingStart == 0)
		{
			jumpingStart = GetTickCount();
		}
	}
};