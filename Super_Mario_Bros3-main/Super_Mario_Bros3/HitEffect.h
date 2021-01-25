#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScene.h"
#include "define.h"







class CHitEffect : public CGameObject
{
	bool isUsed = false;
	bool isAppear = false;

	int type;

	DWORD timing_start = 0;

	bool isAllowToShowHitEffectTurnTail = false;
	bool isAllowToShowHitEffectFireBullet = false;

public:

	CHitEffect(int ctype);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = (DWORD)GetTickCount64();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	bool GetIsAllowToShowHitEffectTurnTail()
	{
		return isAllowToShowHitEffectTurnTail;
	}
	void SetIsAllowToShowHitEffectTurnTail(bool isAllowToShowHitEffectTurnTailBool)
	{
		isAllowToShowHitEffectTurnTail = isAllowToShowHitEffectTurnTailBool;
	}
	bool GetIsAllowToShowHitEffectFireBullet()
	{
		return isAllowToShowHitEffectFireBullet;
	}
	void SetIsAllowToShowHitEffectFireBullet(bool isAllowToShowHitEffectFireBulletBool)
	{
		isAllowToShowHitEffectFireBullet = isAllowToShowHitEffectFireBulletBool;
	}

};