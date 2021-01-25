#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "Leaf.h"





class CQuestionBrick : public CGameObject
{
	int type;
	bool isAlive = true;
	bool isUsed = false;

	bool isAllowQuestionBrickSlide = false;

	int time_Y_Up = 0;
	bool isUp = false;

	bool isAllowToShowScore = false;
	DWORD timing_score;

	int life;

	bool controlMultipleCoin = false;
	bool isAllowToShowMultipleCoin = false;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CQuestionBrick(int ctype);
	virtual void SetState(int state);
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
	void SetType(int Ctype)
	{
		type = Ctype;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUp()
	{
		return isUp;
	}
	void SetIsUp(bool isUpBool)
	{
		isUp = isUpBool;
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
	void SetLifeDown()
	{
		if (life >= 0)
			life--;
	}
	int GetLife()
	{
		return life;
	}
	void SetLife(int lifeInt)
	{
		life = lifeInt;
	}
	bool GetIsAllowQuestionBrickSlide()
	{
		return isAllowQuestionBrickSlide;
	}
	void SetIsAllowQuestionBrickSlide(bool isAllowQuestionBrickSlideBool)
	{
		isAllowQuestionBrickSlide = isAllowQuestionBrickSlideBool;
	}
	bool GetControlMultipleCoin()
	{
		return controlMultipleCoin;
	}
	void SetControlMultipleCoin(bool controlMultipleCoinBool)
	{
		controlMultipleCoin = controlMultipleCoinBool;
	}
	bool GetIsAllowToShowMultipleCoin()
	{
		return isAllowToShowMultipleCoin;
	}
	void SetIsAllowToShowMultipleCoin(bool isAllowToShowMultipleCoinBool)
	{
		isAllowToShowMultipleCoin = isAllowToShowMultipleCoinBool;
	}
};