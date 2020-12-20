#pragma once
#include "GameObject.h"


class PIPE : public CGameObject
{
	int type;
public:
	PIPE(int ctype);
	int GetType()
	{
		return type;
	}
	void SetType(int ctype)
	{
		type = ctype;
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};