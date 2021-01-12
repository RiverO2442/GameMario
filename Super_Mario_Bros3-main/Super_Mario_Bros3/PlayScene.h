#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Scene.h"
#include "HUD.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Coin.h"
#include "Pipe.h"
#include "RECT.h"
#include "NoCollisionObject.h"
#include "FlowerBullet.h"
#include "QuestionBrick.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "FIREBALL.h"
#include "Flower.h"
#include "FlowerBullet.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "BreakableBrick.h"
#include "Bell.h"
#include "Special_Item.h"
#include "WordsEndScene.h"
#include "MovingRock.h"
#include "define.h"
#include "Cell.h"
#include <iostream>
#include <fstream>
#include "NewMapCam.h"

#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024

class CGrid
{
	int numRow, numCol;
	int cellWidth;
	int  cellHeight;
	Cell** cells;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CGrid(LPCWSTR filePath);
	void GetObjects(vector<LPGAMEOBJECT>& listObject, int playerX, int playerY);
	void Load(LPCWSTR filePath);
	void Unload();
};

class CPlayScene : public CScene
{
protected:
	CMario* player;			// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects;

	vector<CHUD*>  timers;
	vector<CHUD*>  scores;
	vector<CHUD*>  moneys;
	vector<CHUD*>  normarl_stacks;
	vector<CHUD*>  items;
	CHUD* max_stack;

	CGrid* grid;

	int cam_state;

	vector<CNewMapCam*> new_map_cams;

	DWORD time_counter = 0;

	DWORD time_cam_move = 0;

	float UpdateCamMoveX(DWORD dt);

	int time_picker = 300;

	float cam_x_pre = 0;
	float cam_y_pre = 0;

	int lv_pre = 1;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_GRID(string line);



public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	void StartTimeCamMove()
	{
		if (time_cam_move == 0)
			time_cam_move = GetTickCount();
	}
	void SetCamState(int camStateInt)
	{
		cam_state = camStateInt;
	}
	int GetCamState()
	{
		return cam_state;
	}

	void SetTimeDown()
	{
		if (time_picker >= 50)
		{
			this->time_picker -= 50;
		}
		else
		{
			this->time_picker -= (this->time_picker % 50);
		}
	}
	int GetTimePicker()
	{
		return time_picker;
	}

	void SetTimePicker(int VALUE)
	{
		time_picker = VALUE;
	}
	void StartTimeCounter()
	{
		if (time_counter == 0)
			time_counter = GetTickCount();
	}

	vector<CHUD*> GetTimers()
	{
		return timers;
	}
	vector<CHUD*> GetScores()
	{
		return scores;
	}
	vector<CHUD*> GetMoneys()
	{
		return moneys;
	}
	vector<CHUD*> GetNormalStacks()
	{
		return normarl_stacks;
	}
	CHUD* GetMaxStack()
	{
		return max_stack;
	}
	vector<CHUD*> GetItems()
	{
		return items;
	}
	float GetCamXPre()
	{
		return cam_x_pre;
	}
	float GetCamYPre()
	{
		return cam_y_pre;
	}
	void SetLvPre(int value)
	{
		lv_pre = value;
	}
	int GetLvPre()
	{
		return lv_pre;
	}

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};

};

