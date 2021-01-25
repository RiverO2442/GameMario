#include <iostream>
#include <fstream>
#include "PlayScene.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	cam_state = 1;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);
	if(grid == NULL)
	grid = new CGrid(file_path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{

	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int renderLayer = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	CHUD* HUD_items = NULL;

	CNewMapCam* new_map_cam = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(1, x, y);
		player = (CMario*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_HIT_EFFECT_TURN_TAIL:
		obj = new CHitEffect(HIT_EFFECT_TURN_TAIL);
		break;
	case OBJECT_TYPE_HIT_EFFECT_FIRE_BULLET:
		obj = new CHitEffect(HIT_EFFECT_FIRE_BULLET);
		objects.push_back(obj);
		break;
	case OBJECT_TYPE_BOOMERANG_ENEMY:
		obj = new CBoomerangEnemy();
		break;
	case OBJECT_TYPE_BOOMERANG:
	{
		int boomerang_id = (int)(int)atof(tokens[4].c_str());
		obj = new CBoomerang(boomerang_id);
	}
	break;
	case OBJECT_TYPE_MOVING_HORIZONTAL_RECTANGLE:
	{
		int moving_horizontal_rectangle_id = (int)(int)atof(tokens[4].c_str());
		obj = new CMovingHorizontalRectangle(moving_horizontal_rectangle_id);
	}
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_MULTIPLE_LIFE:
		obj = new CQuestionBrick(QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE);
		break;
	case OBJECT_TYPE_SCORE_AND_1LV:
		obj = new CScore();
		scores_panel.push_back(obj);
		break;
	case OBJECT_TYPE_NEW_MAP_CAM:
	{
		float y_limit = (float)atof(tokens[4].c_str());
		float y_start = (float)atof(tokens[5].c_str());
		new_map_cam = new CNewMapCam(ani_set_id, x, y, y_limit, y_start);
		new_map_cams.push_back(new_map_cam);
	}
	break;
	case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(888, 3); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(999, 3); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_XANH_WALK: obj = new CKoopas(111, 3); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRECT(); break;
	case OBJECT_TYPE_COIN_NORMAL: obj = new CCoin(222); break;
	case OBJECT_TYPE_COIN_CAN_MOVE: obj = new CCoin(333); break;
	case OBJECT_TYPE_PIPE: obj = new PIPE(100); break;
	case OBJECT_TYPE_PIPE_DOWN: obj = new PIPE(200); break;
	case OBJECT_TYPE_PIPE_UP: obj = new PIPE(300); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObject(3, 1); break;
	case OBJECT_TYPE_KOOPAS_XANH_BAY: obj = new CKoopas(222, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(333, 3); break;
	case OBJECT_TYPE_FIREBALL:     obj = new FIREBALL(); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlower(100); break;
	case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlower(200); break;
	case OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT:   obj = new CFlower(300); break;
	case OBJECT_TYPE_FLOWER_BULLET:	   obj = new CFlowerBullet(); break;
	case OBJECT_TYPE_QUESTION_BRICK: obj = new CQuestionBrick(777); break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_ITEM: obj = new CQuestionBrick(888); break;
	case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(567); break;
	case OBJECT_TYPE_MUSHROOM_GREEN:   obj = new CMushRoom(678); break;
	case OBJECT_TYPE_BREAKABLE_BRICK: obj = new CBreakableBrick(); break;
	case OBJECT_TYPE_BELL: obj = new CBell(); break;
	case OBJECT_TYPE_BLACK_BLACK: obj = new CHUD(1000); break;
	case OBJECT_TYPE_SPECIAL_ITEM: obj = new CSpecial_Item(); break;
	case OBJECT_TYPE_WORDS_END_SCENE_COURSE_CLEAR: obj = new CWordsEndScene(111); break;
	case OBJECT_TYPE_WORDS_END_SCENE_YOU_GOT_A_CARD: obj = new CWordsEndScene(222); break;
	case OBJECT_TYPE_WORDS_END_SCENE_ITEM: obj = new CWordsEndScene(333); break;
	case OBJECT_TYPE_MOVING_ROCK: obj = new CMovingRock(); break;

	case OBJECT_TYPE_HUD_PANEL:
		obj = new CHUD(11);
		break;
	case OBJECT_TYPE_WORLD:
		obj = new CHUD(22);
		break;
	case OBJECT_TYPE_MARIO_LUIGI:
		obj = new CHUD(77);
		break;
	case OBJECT_TYPE_LIFE:
		obj = new CHUD(33);
		break;
	case OBJECT_TYPE_TIME_PICKER:
		HUD_items = new CHUD(44);
		timers.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_SCORE:
		HUD_items = new CHUD(55);
		scores.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_MONEY:
		HUD_items = new CHUD(66);
		moneys.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_NORMAL:
		HUD_items = new CHUD(88);
		normarl_stacks.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_MAX:
		HUD_items = new CHUD(99);
		max_stack = (CHUD*)HUD_items;
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_ITEM:
		HUD_items = new CHUD(100);
		items.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_PORTAL:
	{
		int portal_id = (int)atof(tokens[4].c_str());
		float arrive_position_x = (float)atof(tokens[5].c_str());
		float arrive_position_y = (float)atof(tokens[6].c_str());
		obj = new CPortal(portal_id, arrive_position_x, arrive_position_y);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		obj->SetOrigin(x, y, obj->GetState());
		obj->SetisOriginObj(true);
		obj->SetrenderLayer(renderLayer);
		objects.push_back(obj);
	}
	if (HUD_items != NULL)
		HUD_items->SetAnimationSet(ani_set);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}

		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }

		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

bool CPlayScene::IsInUseArea(float Ox, float Oy)
{
	float CamX, CamY;

	CamX = (float)CGame::GetInstance()->GetCamX();

	CamY = (float)CGame::GetInstance()->GetCamY();

	if (((CamX - CAM_X_BONUS < Ox) && (Ox < CamX + IN_USE_WIDTH)) && ((CamY < Oy) && (Oy < CamY + IN_USE_HEIGHT)))
		return true;
	return false;
}

bool CPlayScene::CheckCamY()
{
	float px, py;
	player->GetPosition(px, py);
	if (py > new_map_cams[cam_state - 1]->GetCamYLimit())
		camYMove = false;
	if (py < new_map_cams[cam_state - 1]->GetCamYLimit() || player->GetIsFlying())
		camYMove = true;
	return camYMove;
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();

	CGame* game = CGame::GetInstance();

	if(game->GetCamX() == CAM_START && game->GetCamY() == CAM_START)
	CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(), (int)new_map_cams[cam_state - 1]->GetYStart());

		lv_pre = player->GetLevel();

		StartTimeCounter();

		float cx, cy;

		player->GetPosition(cx, cy);

		cam_x_pre = (float)game->GetCamX();
		cam_y_pre = (float)game->GetCamY();

		if (id == SCENE_1_1_ID)
		{
			cx -= game->GetScreenWidth() / 2;
			if (player->x >= (game->GetScreenWidth() / 2 + new_map_cams[cam_state - 1]->GetStartCamX()))
			{
				if (CheckCamY() || player->GetIsAtTheTunnel())
				{
					cy -= game->GetScreenHeight() / 2;
					CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
				}
				else
				{
					CGame::GetInstance()->SetCamPos((int)cx, (int)new_map_cams[cam_state - 1]->GetYStart());
				}
			}
			else
			{
				if (CheckCamY() || player->GetIsAtTheTunnel())
				{
					cy -= game->GetScreenHeight() / 2;
					CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
				}
				else
				{
					CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(), (int)new_map_cams[cam_state - 1]->GetYStart());
				}
			}
		}
		else
		if(id == SCENE_1_4_ID)
		if (cam_state == 1)
		{
			StartTimeCamMove();
			if (time_cam_move != 0)
			{
				if ((DWORD)GetTickCount64() - time_cam_move >= 10)
				{
					time_cam_move = 0;
					float cam_x_update = UpdateCamMoveX(dt);
					CGame::GetInstance()->SetCamPos((int)cam_x_update, (int)new_map_cams[cam_state - 1]->GetYStart());
				}
			}
		}
		else
		{
			if (cam_state == 2)
			{
				cx -= game->GetScreenWidth() / 2;
				if (player->x >= (game->GetScreenWidth() / 2 + new_map_cams[cam_state - 1]->GetStartCamX()))
				{
					if (CheckCamY())
					{
						cy -= game->GetScreenHeight() / 2;
						CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
					}
					else
					{
						CGame::GetInstance()->SetCamPos((int)cx, (int)new_map_cams[cam_state - 1]->GetYStart());
					}
				}
				else
				{
					if (CheckCamY())
					{
						cy -= game->GetScreenHeight() / 2;
						CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
					}
					else
					{
						CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(), (int)new_map_cams[cam_state - 1]->GetYStart());
					}
				}
			}
		}
		if (game->GetCamX() >= new_map_cams[cam_state - 1]->GetEndCamX())
			game->SetCamX((int)new_map_cams[cam_state - 1]->GetEndCamX());
		DebugOut1(L"%d \n", cam_state);
		cx = (float)game->GetCamX();

		cy = (float)game->GetCamY();
		
		grid->GetObjects(objects, (int)cx, (int)cy);

		for (size_t i = 0; i < objects.size(); i++)
		{
			float Ox, Oy;
			objects[i]->GetPosition(Ox, Oy);
			if (!IsInUseArea(Ox, Oy) && !objects[i]->GetisOriginObj())
			{
				objects[i]->SetActive(false);
				objects.erase(objects.begin() + i);
			}
		}
			
		DebugOut1(L"So Luong CooBJ %d \n", objects.size());
		//DebugOut1(L"So Luong CooBJ %d \n", objects.size());

		vector<LPGAMEOBJECT> coObjects;

		//for (size_t i = 0; i < objects.size(); i++)
		//{
		//	if(!dynamic_cast<CBrick*>(objects[i]))
		//	coObjects[i]->Update(dt, &objects);
		//}

		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &objects);
		}

		//DebugOut1(L"So Luong CooBJ %d \n", objects.size());

		if ((DWORD)GetTickCount64() - time_counter >= 1000 && time_picker > 0 && player->GetControl() != LOSE_ALL_CONTROL)
		{
			time_picker--;
			time_counter = 0;
		}

		for (size_t i = 0; i < timers.size(); i++)
		{
			timers[i]->Update(dt, NULL);
		}

		for (size_t i = 0; i < scores.size(); i++)
		{
			scores[i]->Update(dt, NULL);
		}

		for (size_t i = 0; i < moneys.size(); i++)
		{
			moneys[i]->Update(dt, NULL);
		}

		for (size_t i = 0; i < normarl_stacks.size(); i++)
		{
			normarl_stacks[i]->Update(dt, NULL);
		}

		for (size_t i = 0; i < items.size(); i++)
		{
			items[i]->Update(dt, NULL);
		}

		max_stack->Update(dt, NULL);

		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (player == NULL) return;

		// Update camera to follow mario	
}

void CPlayScene::Render()
{

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();

	CGame* game = CGame::GetInstance();

	float cx, cy;

	vector<LPGAMEOBJECT> render[MAX_RENDER_LAYER];

	for (size_t i = 0; i < objects.size(); i++)
	{
		render[objects[i]->GetrenderLayer() - 1].push_back(objects[i]);
	}

	player->GetPosition(cx, cy);

	if (map)
	{
		this->map->Render(game->GetCamX() , game->GetCamY());
	}

		for (int i = 0; i < MAX_RENDER_LAYER; i++)
		{
			for (size_t j = 0; j < render[i].size(); j++)
				render[i][j]->Render();
		}

		for (size_t i = 0; i < timers.size(); i++)
		{
			timers[i]->Render(i);
		}

		for (size_t i = 0; i < scores.size(); i++)
		{
			scores[i]->Render(i);
		}

		for (size_t i = 0; i < moneys.size(); i++)
		{
			moneys[i]->Render(i);
		}

		for (size_t i = 0; i < normarl_stacks.size(); i++)
		{
			normarl_stacks[i]->Render(i);
		}

		max_stack->Render();

		for (size_t i = 0; i < items.size(); i++)
		{
			items[i]->Render(i);
		}
}

/*
	Unload current scene
*/

void CPlayScene::Unload()
{
	//
	//for (int i = 0; i < objects.size(); i++)
	//	delete objects[i];

	for (size_t i = 0; i < scores.size(); i++)
	{
		delete scores[i];
	}

	for (size_t i = 0; i < moneys.size(); i++)
	{
		delete moneys[i];
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		delete items[i];
	}

	for (size_t i = 0; i < timers.size(); i++)
	{
		delete timers[i];
	}

	for (size_t i = 0; i < normarl_stacks.size(); i++)
	{
		delete normarl_stacks[i];
	}

	for (size_t i = 0; i < new_map_cams.size(); i++)
	{
		delete new_map_cams[i];
	}

	new_map_cams.clear();
	items.clear();
	moneys.clear();
	scores.clear();
	objects.clear();
	normarl_stacks.clear();
	timers.clear();

	player = NULL;

	cam_state = 1;

	delete map;

	map = nullptr;

	grid->Unload();

	grid = nullptr;

	delete grid;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() != MARIO_STATE_PIPE_UPPING && mario->GetState() != MARIO_STATE_PIPE_DOWNING && !mario->GetIsTransform() && !mario->GetIsSmokeTransform())
	{
		switch (KeyCode)
		{
			/*case DIK_DOWN:
				mario->FIXPS(0, 9);
				break;*/
		case DIK_B:
			mario->SetLevel(MARIO_LEVEL_BIG);
			break;
		case DIK_L:
			float x, y;
			mario->GetPosition(x,y);
			mario->SetPosition(x + mario->nx*200, 220);
			break;
		case DIK_M:
			mario->SetLevel(MARIO_LEVEL_SMALL);
			break;
		case DIK_F:
			mario->SetLevel(MARIO_LEVEL_FIRE);
			break;
		case DIK_T:
			mario->SetLevel(MARIO_LEVEL_TAIL);
			break;
		case DIK_S:
			if (mario->GetIsJumping() == false)
			{
				mario->SetState(MARIO_STATE_JUMP);
				mario->SetIsJumping(true);
				mario->Start_jumping_long();
				mario->SetIsOnMovingHorizontalRectangle(false);
				mario->SetMarioMovingHorizotalRecID(-1);
				mario->SetControlMarioColliWithMovingRec(false);
			}
			break;
		case DIK_A:
			mario->SetisHolding(true);
			if (mario->GetLevel() == MARIO_LEVEL_TAIL)
				mario->SetIsSpining(true);
			mario->StartSpining();
			mario->SetisFiring(true);
			if (mario->GetLevel() == MARIO_LEVEL_FIRE)
				mario->startFiring();
			break;
		case DIK_R:
			CGame::GetInstance()->SwitchScene(WORLD_MAP_ID);
			break;
		}
	}
}

float CPlayScene::UpdateCamMoveX(DWORD dt)
{

	int cam_x_end_temp = (int)new_map_cams.at(cam_state - 1)->GetEndCamX();

	int cam_x_game = CGame::GetInstance()->GetCamX();

	if (cam_x_game < cam_x_end_temp)
	{
		cam_x_game += (int)(MOVE_CAM_X_VX * dt);
		return (float)cam_x_game;
	}
	else
	{
		return (float)cam_x_end_temp;
	}


}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() != MARIO_STATE_PIPE_UPPING && mario->GetState() != MARIO_STATE_PIPE_DOWNING && !mario->GetIsTransform() && !mario->GetIsSmokeTransform())
	{
		switch (KeyCode)
		{
			/*case DIK_DOWN:
				mario->FIXPS(0, 9);
				break;*/
		case DIK_A:
			mario->SetisHolding(false);
			mario->SetisFiring(false);
			mario->SetisAlreadyFired(false);
			break;
		case DIK_S:
			mario->Setcanjumplong(false);
			mario->Setmario_time_jumpt(0);
			mario->Setjumping_long_start(0);
			mario->main_jump = false;
			mario->SetIsFlying(false);
			mario->SetIsFalling(false);
			break;
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() != MARIO_STATE_PIPE_UPPING && mario->GetState() != MARIO_STATE_PIPE_DOWNING && !mario->GetIsTransform() && !mario->GetIsSmokeTransform() && mario->GetControl() != LOSE_ALL_CONTROL)
	{
		// disable control key when Mario die 
		if (mario->GetState() == MARIO_STATE_DIE) return;
		if (game->IsKeyDown(DIK_S))
		{
			if (mario->Getcanjumplong())
			{
				mario->main_jump = true;
			}
			if (mario->GetspeedLevel() == MARIO_MAX_SPEED_LEVEL)
				mario->SetCanFly(true);
			//else
			//{
			//	mario->Setjumping_long_start
			//}
			if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetCanFly())
			{
				mario->SetControl(0);
				if (mario->nx > 0)
				{
					mario->SetState(MARIO_STATE_FLYING_RIGHT);
				}
				else
				{
					mario->SetState(MARIO_STATE_FLYING_LEFT);
				}
				mario->SetIsFlying(true);
			}
			else
			{
				if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->vy > 0)
				{
					if (mario->Getspeeddown_start() == 0)
						mario->StartSpeeddown();
					mario->DEACCELERETING();
					mario->SetState(MARIO_STATE_FALLING_DOWN);
					mario->SetIsFalling(true);
					mario->SetCanFall(true);
				}

			}
		}

		if (game->IsKeyDown(DIK_RIGHT) && mario->GetControl() != MARIO_LOCK_RIGHT)
		{
			mario->SetControl(0);
			if (game->IsKeyDown(DIK_A))
			{
				if (mario->Getspeedup_start() == 0)
					mario->StartSpeedup();
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
				mario->ACCELERETING();
			}
			else
			{
				if (mario->Getspeeddown_start() == 0)
					mario->StartSpeeddown();
				if (!mario->GetCanFly())
					mario->DEACCELERETING();
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
		}
		else if (game->IsKeyDown(DIK_LEFT) && mario->GetControl() != MARIO_LOCK_LEFT)
		{
			mario->SetControl(0);
			if (game->IsKeyDown(DIK_A))
			{

				if (mario->Getspeedup_start() == 0)
					mario->StartSpeedup();
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
				mario->ACCELERETING();
			}
			else
			{
				if (mario->Getspeeddown_start() == 0)
					mario->StartSpeeddown();
				if (!mario->GetCanFly())
					mario->DEACCELERETING();
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			if (mario->GetCanPipeUpping())
			{
				mario->SetState(MARIO_STATE_PIPE_UPPING);
				mario->StartPipeUpping();
				return;
			}
		}
		else
		{
			if ((mario->nx > 0 && mario->vx <= 0) || (mario->nx < 0 && mario->vx >= 0))
			{
				if (mario->Getspeeddown_start() == 0)
					mario->StartSpeeddown();
				if (!mario->GetCanFly())
					mario->DEACCELERETING();
				mario->SetState(MARIO_STATE_IDLE);
			}
			if (mario->vx != 0)
			{
				if (mario->Getspeeddown_start() == 0)
					mario->StartSpeeddown();
				if (!mario->GetCanFly())
					mario->DEACCELERETING();
				mario->SetState(MARIO_STATE_SPEED_DOWN);
			}
			//mario->SetState(MARIO_STATE_IDLE);
			if (game->IsKeyDown(DIK_DOWN))    //Sit down
			{
				mario->SetisSitting(true);
				if (mario->GetCanPipeDowning())
				{
					mario->SetState(MARIO_STATE_PIPE_DOWNING);
					mario->StartPipeDowning();
					return;
				}
			}
			else
				mario->SetisSitting(false);
		}
		if (game->IsKeyDown(DIK_A))
		{
			if (mario->GetLevel() == MARIO_LEVEL_FIRE)
				mario->SetisFiring(true);
		}
	}

}