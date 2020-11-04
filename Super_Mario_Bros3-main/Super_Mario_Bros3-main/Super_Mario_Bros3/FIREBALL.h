#pragma once
#include "GameObject.h"
#define FIREBALL_SPEED 0.15f;

#define FIREBALL_BBOX_WIDTH	7
#define FIREBALL_BBOX_HEIGHT 9

#define	FIREBALL_STATE_DIE	90
#define FIREBALL_STATE_FLYING 100


#define FIREBALL_ANI_FLYING 0

#define FIREBALL_GRAVITY	0.0008f
#define FIREBALL_RESET_TIME 5000

class FIREBALL : public CGameObject
{
	DWORD reset_start;
	bool isUsed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void StartReset() { reset_start = GetTickCount(); }

public:
	float upBoudary;
	FIREBALL();
	void SetisUsed(bool value) { isUsed = value; }
	bool GetisUsed() { return isUsed; }
	virtual void SetState(int state);
};

