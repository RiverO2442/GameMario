#pragma once
#include "GameObject.h"
#include "define.h"

class CMario : public CGameObject
{
	int time_mario = 0;

	int type;
	bool isAppear = true;

	int heightLimit;
	bool isBraking;
	int speedLevel = 1;
	int level;
	int untouchable;
	DWORD flying_start = 0;
	DWORD braking_start;
	DWORD untouchable_start;
	DWORD speedup_start;
	DWORD kicking_start;
	DWORD spining_start;
	DWORD hitted_start = 0;
	bool isSpining = true;
	bool isJumping = 0;
	bool isFlying = false;
	bool isFalling = false;
	bool canFall = false;
	bool canFly = false;
	bool isKicking = 0;
	bool isHolding = 0;
	bool renderHolding = 0;
	bool isFiring = 0;
	bool isAlreadyFired = 0;
	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	bool GetIsAppear()
	{
		return isAppear;
	}
	void SetIsAppear(bool isAppearBool)
	{
		this->isAppear = isAppearBool;
	}
	void SetrenderHolding(bool value)
	{
		this->renderHolding = value;
	}
	bool GetrenderHolding()
	{
		return renderHolding;
	}
	bool GetIsFlying()
	{
		return isFlying;
	}
	void SetIsFlying(bool isFallingBool)
	{
		this->isFlying = isFallingBool;
	}
	bool GetIsFalling()
	{
		return isFalling;
	}
	void SetIsFalling(bool isFallingBool)
	{
		this->isFalling = isFallingBool;
	}
	bool GetCanFly()
	{
		return canFly;
	}
	void SetCanFly(bool flyBool)
	{
		canFly = flyBool;
		return;
	}
	bool GetCanFall()
	{
		return canFall;
	}
	void SetCanFall(bool fallBool)
	{
		canFall = fallBool;
		return;
	}
	void SetisBraking(bool value) { isBraking = value; }
	bool GetisBraking() { return isBraking; }

	void SetisFiring(bool value) { isFiring = value; }
	bool GetisFiring() { return isFiring; }

	void SetisAlreadyFired(bool value) { isAlreadyFired = value; }
	bool GetisAlreadyFired() { return isAlreadyFired; }

	void SetisHolding(bool value) { isHolding = value; }
	bool GetisHolding() { return isHolding; }

	CMario(int ctype, float x = 0.0f, float y = 0.0f);

	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);

	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);

	bool GetIsJumping() { return isJumping; }
	void SetIsJumping(bool value) { isJumping = value; }

	bool GetUnTounchable() { return untouchable; }
	void SetUnTounchable(bool value) { untouchable = value; }

	bool GetIsKicking() { return isKicking; }
	void SetIsKicking(bool value) { isKicking = value; }

	bool GetIsSpining() { return isSpining; }
	void SetIsSpining(bool value) { isSpining = value; }

	int GetspeedLevel() { return speedLevel; }
	void SetspeedLevel(int value) { speedLevel = value; }

	DWORD GetFlyingStart()
	{
		return flying_start;
	}

	DWORD Getspeedup_start()
	{
		return speedup_start;
	}
	void Setspeedup_start(int value)
	{
		speedup_start = value;
	}
	void RenderBoundingBox();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	int GetLevel() {
		return level;
	};
	void SetLevel(int l)
	{
		LvChanging();
		level = l;
	}
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void StartKicking() { kicking_start = GetTickCount(); }

	void StartSpining() { spining_start = GetTickCount(); }

	void StartSpeedup() { speedup_start = GetTickCount(); }

	void StartFlying() { flying_start = GetTickCount(); }

	void StartHitted() { hitted_start = GetTickCount(); }

	void Reset();

	void FIXPS(int PSx, int PSy);

	void LvChanging();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool BrakingCalculation()
	{
		if (nx * vx < 0)
		{
			if (vx < 0)
			{
				vx += 0.008;
			}
			else
			{
				vx -= 0.008;
			}
			isBraking = true;
			SetspeedLevel(0);
			return true;
		}
		else
		{
			isBraking = false;
		}
		return false;
	}

};