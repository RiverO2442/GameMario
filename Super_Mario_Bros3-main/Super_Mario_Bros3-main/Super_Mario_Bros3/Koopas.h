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

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_SHELL_UP	3
#define KOOPAS_ANI_SHELL_DOWN	4
#define KOOPAS_ANI_SPINNING 5
#define KOOPAS_GRAVITY	0.002f

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CKoopas();
	virtual void SetState(int state);
};