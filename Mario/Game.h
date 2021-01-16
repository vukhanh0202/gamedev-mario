#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "KeyEvent.h"
#include <WinUser.h>
#include "Scene.h"
#include "PlayScene.h"

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

using namespace std;


class Game
{
	static Game * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKeyEventHandler keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;
	unordered_map<int, LPScene> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
public:
	int level;
	int point;
	int score;
	boolean permitLoad;
	vector<int> rewards;
	vector<int> rounds;
	vector<BackGround*> imageDone;

	Game() { level = MARIO_LEVEL_SUPER_BIG, point = 0; score = 0; permitLoad = false; }
	void SetValueStore(int level, int point, int score) {
		this->level = level;
		this->point = point;
		this->score = score;
	}
	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void Init(HWND hWnd);
	void InitKeyboard();
	void SetKeyHandler(LPKeyEventHandler handler) { keyHandler = handler; }

	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPScene GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	void SetCamPosition(double x, double y) { cam_x = x; cam_y = y; }
	void GetCamPosition(double &x, double &y) { x = this->cam_x; y = this->cam_y; }

	/*
		Algorithm SweptAABB handling collision
	*/
	static void SweptAABB(
		float moveLeft,			
		float moveTop,			
		float moveRight,		
		float moveBottom,		
		float dx,			
		float dy,			
		float staticLeft,			
		float staticTop,
		float staticRight,
		float staticBottom,
		float &t,
		float &nx,	
		float &ny);

	static Game * GetInstance();


	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	~Game();
};

