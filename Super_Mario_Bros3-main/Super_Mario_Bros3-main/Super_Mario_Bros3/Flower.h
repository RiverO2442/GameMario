#pragma once
#include "GameObject.h"
// Define flower
#define FLOWER_STATE_LEFT_BOTTOM		100
#define FLOWER_STATE_RIGHT_BOTTOM		200
#define FLOWER_STATE_GROWING_UP			300
#define FLOWER_STATE_GROWING_DOWN		400
#define FLOWER_STATE_IDLE				500
#define FLOWER_STATE_DIE				600

#define FLOWER_ANI_RED_LEFT_BOTTOM		0
#define FLOWER_ANI_RED_RIGHT_BOTTOM		1
#define FLOWER_ANI_GREEN_LEFT_BOTTOM	2
#define FLOWER_ANI_GREEN_RIGHT_BOTTOM	3
#define FLOWER_ANI_RED_TOP				4

// Define fire flower
#define FIRE_FLOWER_STATE_DISAPPEAR		100
#define FIRE_FLOWER_STATE_APPEAR		200

#define FLOWER_BBOX_WIDTH  16
#define FLOWER_BBOX_HEIGHT 32

class CFlower : public CGameObject
{
private:
	DWORD timeFiring_start;
	bool isFirstFiring = false;
	bool hasFired = false;


	int posFlower = -1;
public:
	CFlower();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	void SetHasFired(bool value) { hasFired = value; };

	int GetPositionFlower();
	bool GetHasFired() { return hasFired; }
};