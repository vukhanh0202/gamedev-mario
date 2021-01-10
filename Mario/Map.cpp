#include <fstream>
#include <iostream>
#include "Map.h"
#include "Texture.h"
#include "Ground.h"
#include "Game.h"

Map::Map(int _idTileSet, int _totalRowsTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int _totalTiles)
{
	this->TileSet = Textures::GetInstance()->Get(_idTileSet);
	this->TotalRowsOfTileSet = _totalRowsTileSet;
	this->TotalColumnsOfTileSet = _totalColumnsTileSet;
	this->TotalRowsOfMap = _totalRowsMap;
	this->TotalColumnsOfMap = _totalColumnsMap;
	this->TotalTiles = _totalTiles;
	this->CurrentPosition = 0.0f;
}

Map::~Map()
{
	if (TileMap)
	{
		for (int i = 0; i < TotalRowsOfMap; i++)
		{
			delete TileMap[i];
		}
		delete TileMap;
		TileMap = nullptr;
	}
}

void Map::Render()
{
	for (int r = 0; r < TotalRowsOfMap; r++) {
		for (int c = 0; c < TotalColumnsOfMap; c++) {
			Tiles[TileMap[r][c] - 1]->Draw(c * TILE_WIDTH, r * TILE_HEIGHT, 255); //(x,y,alpha)
		}
	}
}

//void Map::Update(DWORD dt, vector<LPGameObject> &coObjects)
//{
//	Game *game = Game::GetInstance();
//	LPScene scene = Game::GetInstance()->GetCurrentScene();
//	PlayScene* playScene = ((PlayScene*)scene);
//
//	for (size_t i = 0; i < playScene->objects.size(); i++)
//	{
//		coObjects.push_back(playScene->objects[i]);
//	}
//}
void Map::ExtractTileFromTileSet()
{
	for (int TileNumber = 0; TileNumber < TotalTiles; TileNumber++) //total tiles = 91
	{
		int left = TileNumber % TotalColumnsOfTileSet * TILE_WIDTH;
		int top = TileNumber / TotalColumnsOfTileSet * TILE_HEIGHT;
		int right = left + TILE_WIDTH;
		int bottom = top + TILE_HEIGHT;
		LPSprite newTile = new Sprite(TileNumber, left, top, right, bottom, TileSet);
		this->Tiles.push_back(newTile);
	}
}
//void Map::PushObject()
//{
//	Game *game = Game::GetInstance();
//	LPScene scene = Game::GetInstance()->GetCurrentScene();
//	AnimationSets * animation_sets = AnimationSets::GetInstance();
//
//	for (int i = 0; i < TotalRowsOfMap; i++)
//	{
//		for (int j = 0; j < TotalColumnsOfMap; j++)
//		{
//			GameObject *obj = NULL;
//			if (TileMap[i][j] != 1) {
//				obj = new Ground();
//			}
//			/*switch (TileMap[i][j])
//			{
//
//			case 85: case 86: case 87:
//				obj = new Ground(); break;
//			default:
//				DebugOut(L"[ERR] Invalid object type: %d\n", TileMap[i][j]);
//			}*/
//			if (obj) {
//				int TileNumber = i * TotalColumnsOfMap + j;
//				int x = TileNumber % TotalColumnsOfTileSet * TILE_WIDTH;
//				int y = TileNumber / TotalColumnsOfTileSet * TILE_HEIGHT;
//
//				obj->SetPosition(x, y);
//
//				//
//				Sprites::GetInstance()->Add(TileMap[i][j], Tiles[TileMap[i][j] - 1]);
//				// animation
//				LPAnimation ani = new Animation();
//
//				int ani_id = TileMap[i][j];
//
//				int sprite_id = TileMap[i][j];
//				int frame_time = 100;
//				ani->Add(sprite_id, frame_time);
//
//				Animations::GetInstance()->Add(ani_id, ani);
//
//				// animation set
//				int ani_set_id = TileMap[i][j];
//
//				LPAnimation_Set s = new AnimationSet();
//
//				s->push_back(ani);
//
//				AnimationSets::GetInstance()->Add(ani_set_id, s);
//
//				//
//				obj->SetAnimationSet(s);
//				((PlayScene*)scene)->pushObject(obj);
//			}
//		}
//	}
//
//}
void Map::LoadMap(LPCWSTR path)
{
	ifstream f;

	f.open(path);

	// init tilemap
	this->TileMap = new int*[TotalRowsOfMap];
	for (int i = 0; i < TotalRowsOfMap; i++)
	{
		TileMap[i] = new int[TotalColumnsOfMap];
		for (int j = 0; j < TotalColumnsOfMap; j++)
		{
			f >> TileMap[i][j];
		}
	}
	f.close();
}

int Map::GetMapWidth()
{
	return TotalColumnsOfMap * TILE_WIDTH;
}

int Map::GetMapHeight()
{
	return TotalRowsOfMap * TILE_HEIGHT;
}
