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
#include "Map.h"
#include "ScoreManager.h"
#include "Score.h"
#include "MovingHorizontalRectangle.h"
#include"Boomerang.h"
#include "BoomerangEnemy.h"
#include "FireFlower.h"
#include "HitEffect.h"

#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024

class CGrid
{
	unsigned int numRow, numCol;
	unsigned int cellWidth;
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

	bool camYMove = false;

	Map* map;

	vector<LPGAMEOBJECT> scores_panel;
	vector<ScoreManager*> scoreMng;
	vector<ScoreManager*> hitMng;
	vector<CHUD*>  timers;
	vector<CHUD*>  scores;
	vector<CHUD*>  moneys;
	vector<CHUD*>  normarl_stacks;
	vector<CHUD*>  items;
	CHUD* max_stack;

	CGrid* grid;

	int cam_state;

	vector<CNewMapCam*> new_map_cams;

	vector<LPGAMEOBJECT> hit_effects_turn_tail;
	vector<LPGAMEOBJECT> hit_effects_fire_bullet;

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
	void _ParseSection_MAP(string line);



public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();

	virtual bool IsInUseArea(float x, float y);

	bool CheckCamY();

	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	void AddhitMng(float x, float y, int score)
	{
		ScoreManager* sm = new ScoreManager(x, y, score);
		this->hitMng.push_back(sm);
	}
	ScoreManager* GethitMng()
	{
		return hitMng.at(0);
	}
	bool CheckhitMng()
	{
		if (hitMng.size() != 0)
			return true;
		return false;
	}
	void DeletehitMng()
	{
		this->hitMng.erase(hitMng.begin());
	}
	void AddScore(float x, float y, int score)
	{
		ScoreManager* sm = new ScoreManager(x, y, score);
		this->scoreMng.push_back(sm);
	}
	ScoreManager* GetScoreManager()
	{
		return scoreMng.at(0);
	}
	bool CheckScoreMng()
	{
		if (scoreMng.size() != 0)
			return true;
		return false;
	}
	void DeleteScore()
	{
		this->scoreMng.erase(scoreMng.begin());
	}
	void StartTimeCamMove()
	{
		if (time_cam_move == 0)
			time_cam_move = (DWORD)GetTickCount64();
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
			time_counter = (DWORD)GetTickCount64();
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

