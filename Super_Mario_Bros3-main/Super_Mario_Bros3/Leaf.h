#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "define.h"




class CLeaf : public CGameObject
{
	bool isAppear = false;

	DWORD upping_start;
	DWORD downing_start = 0;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CLeaf();
	virtual void SetState(int state);
	void StartUpping()
	{
		upping_start = GetTickCount();
	}
	void StartDowning()
	{
		downing_start = GetTickCount();
	}

};