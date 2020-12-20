#include "NoCollisionObject.h"
#include"Game.h"
#include"Scene.h"

CNoCollisionObject::CNoCollisionObject(int scene_id, int ctype)
{
	type = ctype;
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == 1)
	{
		isRender = false;
	}
	else
		isRender = true;
}

void CNoCollisionObject::Render()
{

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == 1)
	{
		CIntroScence* intro_scene = dynamic_cast<CIntroScence*>(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT> objects = intro_scene->GetObjects();
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBackGroundStage*>(objects[i]))
			{
				CBackGroundStage* background_stage = dynamic_cast<CBackGroundStage*>(objects[i]);
				if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
				{
					if (type == NoCollisionObject_TYPE_NUMBER_THREE)
						isRender = true;
				}
			}
		}
	}
	if (isRender)
		animation_set->at(0)->Render(x, y);
	else
		return;
	//RenderBoundingBox();
}

void CNoCollisionObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}