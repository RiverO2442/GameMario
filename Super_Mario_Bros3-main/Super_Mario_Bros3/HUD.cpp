#include "HUD.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"



CHUD::CHUD(int ctype)
{
	type = ctype;

}



void CHUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == SCENE_1_1_ID || id == SCENE_1_4_ID)
	{

		float cam_x = CGame::GetInstance()->GetCamX();
		float cam_y = CGame::GetInstance()->GetCamY();

		float cam_x_pre = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetCamXPre();
		float cam_y_pre = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetCamYPre();


		this->x += cam_x - cam_x_pre;
		this->y += cam_y - cam_y_pre;
	}

}



void CHUD::Render()
{
	int ani = -1;
	CGame* game = CGame::GetInstance();

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == SCENE_1_1_ID)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		int speedlevel = mario->GetspeedLevel();

		if (speedlevel == 7)
		{
			render_max_stack = true;
		}
		else
		{
			render_max_stack = false;
		}
	}

	switch (type)
	{
	case HUD_TYPE_PANEL:
		ani = HUD_TYPE_PANEL_ANI;
		break;
	case HUD_TYPE_WORLD:
		ani = HUD_TYPE_WORLD_ANI;
		break;
	case HUD_TYPE_MARIO_LUIGI:
		ani = HUD_TYPE_MARIO_LUIGI_ANI;
		break;
	case HUD_TYPE_LIFE:
		ani = game->GetLife();
		break;
	case HUD_TYPE_TIME_PICKER:
		ani = 0;
		break;
	case HUD_TYPE_BLACK_BLACK:
		ani = 0;
		break;
	case HUD_TYPE_STACK_MAX:
		if (render_max_stack)
		{
			ani = HUD_TYPE_STACK_MAX_ANI_FILLED;
		}
		else
		{
			ani = HUD_TYPE_STACK_MAX_ANI_EMPTY;
		}
		break;
	case HUD_TYPE_STACK_NORMAL:
		ani = HUD_TYPE_STACK_NORMAL_ANI_EMPTY;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}





void CHUD::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{

	}
}

void CHUD::Render(int id)
{

	int time_picker = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetTimePicker();


	vector<int> renderTimers;

	int hundred = time_picker / 100;
	int dozen = (time_picker % 100) / 10;
	int unit = (time_picker % 100) % 10;

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			renderTimers.push_back(hundred);
		}
		else if (i == 1)
		{
			renderTimers.push_back(dozen);
		}
		else
		{
			renderTimers.push_back(unit);
		}
	}


	int score = CGame::GetInstance()->GetScore();

	vector<int> renderScores;

	int temp;
	int score_temp = score;


	while (score_temp > 0)
	{
		temp = score_temp % 10;
		renderScores.push_back(temp);
		score_temp = score_temp / 10;
	}

	if (score_temp == 0)
	{
		while (renderScores.size() < 7)
		{
			renderScores.push_back(0);
		}
	}


	int money = CGame::GetInstance()->GetMoney();

	if (money >= 10)
	{
		render_recog_money = true;
	}

	vector<int> renderMoneys;

	int money_dozen = (money % 100) / 10;
	int money_unit = (money % 100) % 10;

	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			renderMoneys.push_back(money_dozen);
		}
		else
		{
			renderMoneys.push_back(money_unit);
		}
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int speed_level_count = mario->GetspeedLevel();



	if (id < speed_level_count)
	{
		stack_alive = true;
	}
	else
	{
		stack_alive = false;
	}


	vector<int> items_type = CGame::GetInstance()->GetItemsTypeRender();

	DebugOut(L"Gia tri cua phan tu dau tien trong vector item :%d\n", items_type.at(0));

	if (type == HUD_TYPE_ITEM)
	{
		this->items_type_render = items_type.at(id);
	}

	int ani = -1;
	switch (type)
	{
	case HUD_TYPE_TIME_PICKER:
		ani = renderTimers.at(id);
		break;
	case HUD_TYPE_SCORE:
		ani = renderScores.at(id);
		break;
	case HUD_TYPE_MONEY:
		if (render_recog_money)
		{
			ani = renderMoneys.at(id);
		}
		else
		{
			if (id == 1)
			{
				ani = renderMoneys.at(id);
			}
			else return;
		}
		break;
	case HUD_TYPE_STACK_NORMAL:
		if (stack_alive)
		{
			ani = HUD_TYPE_STACK_NORMAL_ANI_FILLED;
		}
		else
		{
			ani = HUD_TYPE_STACK_NORMAL_ANI_EMPTY;
		}
		break;
	case HUD_TYPE_ITEM:
		switch (items_type_render)
		{
		case 1:
			ani = HUD_TYPE_ITEM_ANI_EMPTY;
			break;
		case 2:
			ani = HUD_TYPE_ITEM_ANI_MUSHROOM;
			break;
		case 3:
			ani = HUD_TYPE_ITEM_ANI_FLOWER;
			break;
		case 4:
			ani = HUD_TYPE_ITEM_ANI_STAR;
			break;
		}
	}
	animation_set->at(ani)->Render(x, y);
}