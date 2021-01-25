#include "WordsEndScene.h"



CWordsEndScene::CWordsEndScene(int ctype)
{
	type = ctype;
	isAppear = false;
}




void CWordsEndScene::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}

void CWordsEndScene::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (mario->GetIsAllowToShowWordsEndScene())
	{
		StartTimeRecog();
		//DebugOut(L"gia tri la %d \n", (DWORD)GetTickCount64() - this->timing_recog_start);
		if (type == WORDS_END_SCENE_TYPE_COURSE_CLEAR)
		{
			if ((DWORD)GetTickCount64() - timing_recog_start >= WORDSENDSCENE_CLEAR_SHOW_TIME)
			{
				this->isAppear = true;
			}
		}
		else if (type == WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD)
		{
			if ((DWORD)GetTickCount64() - timing_recog_start >= WORDSENDSCENE_GOT_A_CARD_SHOW_TIME)
			{
				this->isAppear = true;
			}
		}
		else if (type == WORDS_END_SCENE_TYPE_ITEM)
		{
			if ((DWORD)GetTickCount64() - timing_recog_start >= WORDSENDSCENE_ITEM_SHOW_TIME)
			{
				this->isAppear = true;
			}
		}
	}


}

void CWordsEndScene::Render()
{
	int ani = -1;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (isAppear)
	{
		switch (type)
		{
		case WORDS_END_SCENE_TYPE_COURSE_CLEAR:
			ani = WORDS_END_SCENE_TYPE_COURSE_CLEAR_ANI;
			break;
		case WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD:
			ani = WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD_ANI;
			break;
		case WORDS_END_SCENE_TYPE_ITEM:
			switch (mario->GetWordsEndSceneItemId())
			{
			case SPECIAL_ITEM_STATE_FLOWER_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_FLOWER;
				break;
			case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_MUSHROOM;
				break;
			case SPECIAL_ITEM_STATE_STAR_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_STAR;
				break;
			}
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CWordsEndScene::SetState(int state)
{
	CGameObject::SetState(state);
}

