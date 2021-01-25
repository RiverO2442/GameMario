#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "define.h"





class CMovingHorizontalRectangle : public CGameObject
{
	bool isAppear = false;
	DWORD upping_start = 0;

	int id;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CMovingHorizontalRectangle(int moving_horizontal_rectangle_id);
	virtual void SetState(int state);
	int GetId()
	{
		return this->id;
	}

};