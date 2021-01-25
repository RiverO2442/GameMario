#include "PlayScene.h"

CGrid::CGrid(LPCWSTR filePath)
{
	Load(filePath);
}

void CGrid::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines

	cellWidth = atoi(tokens[0].c_str());
	cellHeight = atoi(tokens[1].c_str());
	numCol = atoi(tokens[2].c_str());
	numRow = atoi(tokens[3].c_str());

	cells = new LPCELL[numCol];
	for (unsigned int i = 0; i < numCol; i++)
	{
		cells[i] = new Cell[numRow];
	}
}

void CGrid::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	if (tokens.size() < 4) return; // skip invalid lines

	int x = atoi(tokens[1].c_str());
	int y = atoi(tokens[2].c_str());

	int cellX = atoi(tokens[5].c_str());
	int cellY = atoi(tokens[6].c_str());

	int type = atoi(tokens[0].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int renderLayer = atoi(tokens[4].c_str());

	CGameObject* obj = NULL;

	switch (type)
	{
	case OBJECT_TYPE_BOOMERANG_ENEMY:
		obj = new CBoomerangEnemy();
		break;
	case OBJECT_TYPE_BOOMERANG:
	{
		int boomerang_id = (int)atof(tokens[7].c_str());
		obj = new CBoomerang(boomerang_id);
	}
	break;
	case OBJECT_TYPE_MOVING_HORIZONTAL_RECTANGLE:
	{
		int moving_horizontal_rectangle_id = (int)atof(tokens[7].c_str());
		obj = new CMovingHorizontalRectangle(moving_horizontal_rectangle_id);
	}break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_MULTIPLE_LIFE:
		obj = new CQuestionBrick(QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE);
		break;
	case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(GOOMBA_NORMAL, 3); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(GOOMBA_RED_FLY, 3); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_XANH_WALK: obj = new CKoopas(KOOPAS_XANH_WALK, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_FLY: obj = new CKoopas(KOOPAS_RED_FLY, 4); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRECT(); break;
	case OBJECT_TYPE_COIN_NORMAL: obj = new CCoin(COIN_NORMAL); break;
	case OBJECT_TYPE_COIN_CAN_MOVE: obj = new CCoin(COIN_CAN_MOVE); break;
	case OBJECT_TYPE_PIPE: obj = new PIPE(PIPE_TYPE_NORMAL); break;
	case OBJECT_TYPE_PIPE_DOWN: obj = new PIPE(PIPE_TYPE_DOWN); break;
	case OBJECT_TYPE_PIPE_UP: obj = new PIPE(PIPE_TYPE_UP); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObject(3, 1); break;
	case OBJECT_TYPE_KOOPAS_XANH_BAY: obj = new CKoopas(KOOPAS_XANH_FLY, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(KOOPAS_RED_WALK, 3); break;
	case OBJECT_TYPE_FIREBALL:     obj = new FIREBALL(); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlower(FLOWER_RED); break;
	case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlower(FLOWER_GREEN); break;
	case OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT:   obj = new CFlower(FLOWER_GREEN_CAN_SHOOT); break;
	case OBJECT_TYPE_FLOWER_BULLET:	   obj = new CFlowerBullet(); break;
	case OBJECT_TYPE_QUESTION_BRICK: obj = new CQuestionBrick(QUESTION_BRICK_HAVE_LEAF); break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_ITEM: obj = new CQuestionBrick(QUESTION_BRICK_JUST_HAVE_MUSHROOM); break;
	case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(MUSHROOM_RED); break;
	case OBJECT_TYPE_MUSHROOM_GREEN:   obj = new CMushRoom(MUSHROOM_GREEN); break;
	case OBJECT_TYPE_BREAKABLE_BRICK: obj = new CBreakableBrick(); break;
	case OBJECT_TYPE_BELL: obj = new CBell(); break;
	case OBJECT_TYPE_BLACK_BLACK: obj = new CHUD(HUD_TYPE_BLACK_BLACK); break;
	case OBJECT_TYPE_SPECIAL_ITEM: obj = new CSpecial_Item(); break;
	case OBJECT_TYPE_WORDS_END_SCENE_COURSE_CLEAR: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_COURSE_CLEAR); break;
	case OBJECT_TYPE_WORDS_END_SCENE_YOU_GOT_A_CARD: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD); break;
	case OBJECT_TYPE_WORDS_END_SCENE_ITEM: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_ITEM); break;
	case OBJECT_TYPE_MOVING_ROCK: obj = new CMovingRock(); break;
	//case OBJECT_TYPE_PORTAL:
	//{
	//	float r = atof(tokens[4].c_str());
	//	float b = atof(tokens[5].c_str());
	//	int scene_id = atoi(tokens[6].c_str());
	//	obj = new CPortal(x, y, r, b, scene_id);
	//}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", type);
		return;
	}

	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{
		int add = 0;
		obj->SetPosition((float)x, (float)y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin((float)x, (float)y, obj->GetState());
		obj->SetrenderLayer(renderLayer);
		cells[cellX][cellY].Add(obj);
	}

}

void CGrid::Load(LPCWSTR filePath)
{
	DebugOut(L"[INFO] Start loading grid resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section = 0;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

void CGrid::GetObjects(vector<LPGAMEOBJECT>& listObject, int CamX, int CamY)
{
	//listObject.clear();

	unsigned int  left, top, right, bottom;
	unsigned int i, j, k;

	left = ((CamX) / cellWidth);

	right = (CamX + IN_USE_WIDTH) / cellWidth;

	top = (CamY) / cellHeight;

	bottom = (CamY + IN_USE_HEIGHT) / cellHeight;

	LPGAMEOBJECT obj = NULL;

	if (right < 0 || left > numCol || bottom < 0 && top > numRow)
	{
		return;
	}

	if (right > numCol)
	{
		right = numCol;
	}
	if (bottom > numRow)
	{
		bottom = numRow;
	}
	if (left < 0)
	{
		left = 0;
	}
	if (top < 0)
	{
		top = 0;
	}

	for (i = left; i <= right; i++)
	{
		for (j = top; j <= bottom; j++)
		{
			if (!cells[i][j].GetListObjects().empty())
			{
				for (k = 0; k < cells[i][j].GetListObjects().size(); k++)
				{
					if (!cells[i][j].GetListObjects().at(k)->Actived)
					{
						float Ox, Oy;
						cells[i][j].GetListObjects().at(k)->GetOriginLocation(Ox, Oy);
						if (!((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->IsInUseArea(Ox, Oy) /*&& cells[i][j].GetListObjects().at(k)->GetisAlive()*/)
								cells[i][j].GetListObjects().at(k)->reset();
						listObject.push_back(cells[i][j].GetListObjects().at(k));
						cells[i][j].GetListObjects().at(k)->SetActive(true);
					}
				}
			}
		}
	}
}

void CGrid::Unload()
{
	if(this != nullptr)
	if (cells)
	{
		for (unsigned int i = 0; i < numCol; i++)
		{
			for (unsigned int j = 0; j < numRow; j++)
			{
				cells[i][j].Unload();
			}
		}
		delete cells;
		cells = NULL;
	}
}