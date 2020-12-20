#include "MenuGame.h"
#include"Game.h"
#include"Scene.h"

CMenuGame::CMenuGame()
{

}

void CMenuGame::Render()
{
	CIntroScence* intro_scene = dynamic_cast<CIntroScence*>(CGame::GetInstance()->GetCurrentScene());
	int ani = -1;
	bool isShow = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetMenuGame();
	if (isShow)
	{
		isRender = true;
	}

	if (isRender)
	{
		if (intro_scene->menu_game_key_handler)
		{
			ani = MENU_GAME_ANI_2_PLAYER;
		}
		else
		{
			ani = MENU_GAME_ANI_1_PLAYER;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);
}

void CMenuGame::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}