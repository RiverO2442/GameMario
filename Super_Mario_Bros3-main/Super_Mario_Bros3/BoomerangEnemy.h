#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "define.h"




class CBoomerangEnemy : public CGameObject
{
	bool isAllowToHaveBBox = true;

	DWORD time_switch_state = 0;

	bool isAlive = true;

	bool isAllowToRenderThrowAni = false;

	DWORD time_rendering_throw_ani = 0;

	bool isAllowToShowScore = false;
	DWORD timing_score;

	DWORD pre_get_tick_count = 0;
	DWORD sub_time = 0;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CBoomerangEnemy();
	virtual void SetState(int state);

	void StartTimeSwitchingState()
	{
		if (time_switch_state == 0)
		{
			time_switch_state = (DWORD)GetTickCount64();
		}
	}
	void StartTimeRenderingThrowAni()
	{
		if (time_rendering_throw_ani == 0)
		{
			time_rendering_throw_ani = (DWORD)GetTickCount64();
		}
	}
	bool GetIsAlive()
	{
		return isAlive;
	}
	void SetIsAlive(bool isAliveBool)
	{
		isAlive = isAliveBool;
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
		timing_score = (DWORD)GetTickCount64();
	}
	bool GetIsAllowToHaveBBox()
	{
		return isAllowToHaveBBox;
	}
	void SetIsAllowToHaveBBox(bool isAllowToHaveBBoxBool)
	{
		isAllowToHaveBBox = isAllowToHaveBBoxBool;
	}
};