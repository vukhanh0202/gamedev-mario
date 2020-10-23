#include "Sprite.h"
#include "Game.h"
#include "Utils.h"

/*
	Sprite
*/
Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

void Sprite::Draw(float x, float y, int alpha)
{
	Game * game = Game::GetInstance();
	game->Draw(x, y, texture, left, top, right, bottom, alpha);
}

/*
	List Sprites
*/

/*
	Using Singleton Design Pattern to init list sprites
*/
Sprites * Sprites::__instance = NULL;

Sprites *Sprites::GetInstance()
{
	if (__instance == NULL) __instance = new Sprites();
	return __instance;
}


/*
	Add Sprite to ListSprite to manage
*/
void Sprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSprite s = new Sprite(id, left, top, right, bottom, tex);
	listSprites[id] = s;
}

LPSprite Sprites::Get(int id)
{
	return listSprites[id];
}

/*
	Clear all loaded textures
*/
void Sprites::Clear()
{
	for (auto x : listSprites)
	{
		LPSprite s = x.second;
		delete s;
	}

	listSprites.clear();
}
