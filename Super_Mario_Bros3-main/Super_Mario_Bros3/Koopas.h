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
#define KOOPAS_STATE_SHELLING	120
#define KOOPAS_STATE_SPINNING	125
#define KOOPAS_STATE_SHELL_UP	130
#define KOOPAS_STATE_SHELL_DOWN 140
#define KOOPAS_STATE_DIE		200
#define KOOPAS_STATE_DIE_2		400

#define KOOPAS_ANI_WALKING_LEFT		0
#define KOOPAS_ANI_WALKING_RIGHT	1
#define KOOPAS_ANI_FLYING_LEFT		2
#define KOOPAS_ANI_SHELL_DOWN		3
#define KOOPAS_ANI_SHELL_UP			4
#define RED_KOOPAS_ANI_WALKING_LEFT 5
#define RED_KOOPAS_ANI_WALKING_RIGHT 6
#define RED_KOOPAS_ANI_FLYING_LEFT	7
#define RED_KOOPAS_ANI_SHELL_DOWN	8
#define KOOPAS_ANI_SPINNING			9
#define RED_KOOPAS_ANI_SPINNING		10
#define KOOPAS_XANH_ANI_REVIVING	  11
#define KOOPAS_RED_ANI_REVIVING		  12
#define KOOPAS_RED_MAI_ANI_NGUA		  13
#define KOOPAS_XANH_ANI_REVIVING_NGUA	14
#define KOOPAS_RED_ANI_REVIVING_NGUA	15

#define KOOPAS_GRAVITY	0.002f

#define KOOPAS_JUMP_SPEED		0.35f
#define KOOPAS_TIME_JUMPING		900

#define KOOPAS_XANH_WALK	111
#define KOOPAS_XANH_FLY		222
#define KOOPAS_RED_WALK		333
#define KOOPAS_RED_FLY		444

class CKoopas : public CGameObject
{
	int type;
	bool isBeingHold = false;
	float CheckPosition_Y;
	bool CanPullBack = false;
	bool reviveRender = false;
	bool shellUpRender = false;
	bool isKickedRevive = false;
	int dieDirection = -1;
	bool renderRecognization = false;

	DWORD jumpingStart = 0;
	DWORD reviveStart = 0;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

public:
	CKoopas();
	void SetisBeingHold(bool value) { isBeingHold = value; }
	bool GetisBeingHold() { return isBeingHold; }
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	CKoopas(int ctype);
	void StartRevive()
	{
		reviveStart = GetTickCount();
	}
	DWORD GetReviveStart()
	{
		return reviveStart;
	}
	virtual void SetState(int state);
};