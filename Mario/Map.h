#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprite.h"
#include "GameObject.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class Map
{
private:
	int** TileMap;
	vector<LPSprite> Tiles;
	LPDIRECT3DTEXTURE9 TileSet; //map1-1_bank.png
	int TotalColumnsOfMap, TotalRowsOfMap;
	int TotalColumnsOfTileSet, TotalRowsOfTileSet;
	int TotalTiles; //total tiles of tileset

public:
	Map(int _idTileSet, int _totalRowsTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int _totalTiles);
	~Map();
	void Render();
	void Update(DWORD dt, vector<LPGameObject> &coObjects);
	void ExtractTileFromTileSet();
	void LoadMap(LPCWSTR file_path);
	void PushObject();

	int GetMapHeight();
	int GetMapWidth();
};
