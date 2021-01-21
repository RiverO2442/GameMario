#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScene.h"
#include "ScrollingStage.h"
#include "BackGroundStage.h"
#include "Star.h"
#include "MenuGame.h"
#include "NewMapCam.h"
class CIntroScence : public  CScene
{
	int cam_state;

	vector<CNewMapCam*> new_map_cams;

	bool showmenugame = false;
public:
	bool menu_game_key_handler;
protected:
	vector<LPGAMEOBJECT> objects;
	


	DWORD time_count = 0;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	bool GetMenuGame()
	{
		return showmenugame;
	}
	void SetMenuGame(bool value)
	{
		showmenugame = value;
	}
	CIntroScence(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	vector<LPGAMEOBJECT> GetObjects()
	{
		return objects;
	}


	void StartTimeCount()
	{
		if (time_count == 0)
			time_count = GetTickCount();
	}
	~CIntroScence();
};

class CIntroScenceKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

