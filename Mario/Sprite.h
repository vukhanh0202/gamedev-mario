#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

using namespace std;

class Sprite;
typedef Sprite *LPSprite;

class Sprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public:
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	void Draw(float x, float y, int alpha = 255);
};

/*
	Manage sprite database
*/
class Sprites
{
	static Sprites * __instance;

	unordered_map<int, LPSprite> listSprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	LPSprite Get(int id);
	LPSprite &operator[](int id) { return listSprites[id]; }

	static Sprites * GetInstance();
	void Clear();
};


