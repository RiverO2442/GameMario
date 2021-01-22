#pragma once
#pragma once
#include "GameObject.h"
#include "define.h"
#include "Goomba.h"
#include "PlayScene.h"
#include "RECT.h"
#include "FIREBALL.h"
#include "BreakableBrick.h"
#include "BackGroundStage.h"


class CKoopas : public CGameObject
{

	DWORD pre_get_tick_count = 0;
	DWORD sub_time = 0;

	int type;
	bool isBeingHold = false;
	float CheckPosition_Y;
	bool CanPullBack = false;
	bool reviveRender = false;
	bool shellUpRender = false;
	bool isKickedRevive = false;
	bool renderRecognization = false;

	bool isAppear = true;
	bool hitted = false;

	DWORD jumpingStart = 0;
	DWORD reviveStart = 0;

	bool isAllowToSubRecWidth = false;
	DWORD timingSubRecWidth = 0;
	bool isControlSubRecWidth = false;
	

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	CKoopas(int ctype, int scene_id);
	void SetisBeingHold(bool value) { isBeingHold = value; }
	bool GetisBeingHold() { return isBeingHold; }
	bool GetIsAllowToSubRecWidth()
	{
		return isAllowToSubRecWidth;
	}
	void SetIsAllowToSubRecWidth(bool isAllowToSubRecWidthBool)
	{
		isAllowToSubRecWidth = isAllowToSubRecWidthBool;
	}
	void StartTimingSubRecWidth()
	{
		if (timingSubRecWidth == 0)
			timingSubRecWidth = GetTickCount();
	}
	int GetType()
	{
		return type;
	}
	bool GetShellUpRender()
	{
		return shellUpRender;
	}
	void SetShellUpRender(bool shellUpRenderBool)
	{
		this->shellUpRender = shellUpRenderBool;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void SetRenderRegconization(bool renderRegconizationBool)
	{
		this->renderRecognization = renderRegconizationBool;
	}
	bool GetRenderRegconization()
	{
		return renderRecognization;
	}
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