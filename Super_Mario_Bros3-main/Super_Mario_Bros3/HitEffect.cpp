#include "HitEffect.h"



CHitEffect::CHitEffect(int ctype)
{
	this->type = ctype;
	isUsed = false;
	SetState(HIT_EFFECT_STATE_IDLE);
}




void CHitEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CHitEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (this->type == HIT_EFFECT_TURN_TAIL)
	{
		
	}
	else
	{
	}




	x += dx;
	y += dy;



}

void CHitEffect::Render()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	int ani = -1;
	if (isUsed)
	{
		switch (type)
		{
		case HIT_EFFECT_TURN_TAIL:
			if (isAllowToShowHitEffectTurnTail)
			{
				if (mario->GetIsSpining())
				{
					if (nx > 0)
						ani = HIT_EFFECT_TURN_TAIL_ANI_RIGHT;
					else
						ani = HIT_EFFECT_TURN_TAIL_ANI_LEFT;
				}
				else
					return;
			}
			else return;
			break;
		case HIT_EFFECT_FIRE_BULLET:
			if (isAllowToShowHitEffectFireBullet)
			{
				ani = HIT_EFFECT_ANI_FIRE_BULLET;

			}
			else return;
			break;
		}
	}
	else return;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CHitEffect::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HIT_EFFECT_STATE_IDLE:
		vx = vy = 0;
		break;
	}
}

