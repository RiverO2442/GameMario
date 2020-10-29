#pragma once
#pragma once
#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.05f;
#define KOOPAS_SPINNING_SPEED 0.3f;

#define KOOPAS_BBOX_WIDTH 15
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING 100
//#define KOOPAS_STATE_WALKING 110
#define KOOPAS_STATE_SHELLING 120
#define KOOPAS_STATE_SPINNING 125
#define KOOPAS_STATE_SHELL_UP 130
#define KOOPAS_STATE_SHELL_DOWN 140
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DIE_BY_KICK 400

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_FLYING_LEFT 2
#define KOOPAS_ANI_SHELL_DOWN	3
#define KOOPAS_ANI_SHELL_UP	4
#define RED_KOOPAS_ANI_WALKING_LEFT 5
#define RED_KOOPAS_ANI_FLYING_LEFT 6
#define RED_KOOPAS_ANI_SHELL_DOWN	7
#define KOOPAS_ANI_SPINNING 8
#define RED_KOOPAS_ANI_SPINNING 9

#define KOOPAS_GRAVITY	0.002f

#define KOOPAS_XANH_WALK	111
#define KOOPAS_XANH_FLY		222
#define KOOPAS_RED_WALK		333
#define KOOPAS_RED_FLY		444

class CKoopas : public CGameObject
{
	int type;
	bool isBeingHold = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CKoopas();
	void SetisBeingHold(bool value) { isBeingHold = value; }
	bool GetisBeingHold() { return isBeingHold; }
	CKoopas(int ctype);
	virtual void SetState(int state);
};