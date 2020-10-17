#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "KeyBoard.h"

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

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
public:

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void Init(HWND hWnd);
	void InitKeyboard(LPKeyEventHandler handler);

	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void SetCamPosition(float x, float y) { cam_x = x; cam_y = y; }

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

	~Game();
};

