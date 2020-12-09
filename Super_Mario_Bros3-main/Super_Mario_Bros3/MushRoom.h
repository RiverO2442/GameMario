#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "FIREBALL.h"
#include "define.h"





class CMushRoom : public CGameObject
{
	bool isAppear = false;
	int type;
	DWORD upping_start = 0;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CMushRoom(int ctype);
	virtual void SetState(int state);
	
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void StartUpping()
	{
		upping_start = GetTickCount();
	}
	
};