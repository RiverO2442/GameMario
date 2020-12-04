#pragma once
#pragma once
#include "GameObject.h"
#include "define.h"



class CGoomba : public CGameObject
{
	int type;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CGoomba(int ctype);
	CGoomba();
	virtual void SetState(int state);
};