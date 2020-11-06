#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Texture.h"
#include "Sprite.h"
#include "Scene.h"
#include <dinput.h>
#include "Game.h"
#include "Goomba.h"
#include "Brick.h"
#include "Background.h"
#include "Box.h"
#include "Koopa.h"
#include "Ground.h"
#include "FireMario.h"


using namespace std;

PlayScene::PlayScene(int id, LPCWSTR filePath) :
	Scene(id, filePath)
{
	key_handler = new PlaySceneKeyHandler(this);
	mergeObject = false;
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	-2
#define OBJECT_TYPE_BACKGROUND	-1
#define OBJECT_TYPE_GROUND	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPA	3
#define OBJECT_TYPE_FIRE_MARIO	4
#define OBJECT_TYPE_BOX		5

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void PlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	Textures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void PlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = Textures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	Sprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void PlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPAnimation ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	Animations::GetInstance()->Add(ani_id, ani);
}

void PlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPAnimation_Set s = new AnimationSet();

	Animations *animations = Animations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPAnimation ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	AnimationSets::GetInstance()->Add(ani_set_id, s);
}
/*
	Parse a line in section [OBJECTS]
*/
void PlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// Check start merge
	if (line[0] == '!')
	{
		// false -> true and true -> false
		if (mergeObject == false)
		{
			mergeObject = true;				//Start merge
			GameObject *obj = new Boxs();
			objects.push_back(obj);
		}
		else {
			mergeObject = false;			// End merge
			for (size_t i = 0; i < objects.size(); i++)
			{
				if (dynamic_cast<Boxs *>(objects[i]))
				{
					Boxs *boxs = dynamic_cast<Boxs *>(objects[i]);
					if (boxs->getMergeComplete() == false)
					{
						boxs->setMergeComplete(true);
						vector<Box*> listBox = boxs->getListBox();
						float xBoxs, yBoxs;
						listBox[0]->GetPosition(xBoxs, yBoxs);
						boxs->SetPosition(xBoxs, yBoxs);
						boxs->setWidth(BOX_BBOX_WIDTH);
						boxs->setHeight(BOX_BBOX_HEIGHT);


						for (size_t i = 1; i < listBox.size(); i++)
						{
							float xItem, yItem;
							listBox[i]->GetPosition(xItem, yItem);

							// Set width
							if (xItem >= xBoxs)
							{
								boxs->setWidth(xItem - xBoxs + BOX_BBOX_WIDTH);
							}
							else
							{
								float width = boxs->getWidth();
								boxs->setWidth(xBoxs - xItem + width);
								xBoxs = xItem;
							}

							// Set height
							if (yItem < yBoxs)
							{
								float height = boxs->getHeight();
								boxs->setHeight(yBoxs - yItem + height);
								yBoxs = yItem;
							}
							else
							{
								boxs->setHeight(yItem - yBoxs + BOX_BBOX_HEIGHT);
							}
						}

						boxs->SetPosition(xBoxs, yBoxs);
						//objects.push_back(boxs);
					}
				}

			}
		}
	}

	// MERGE BOX
	if (mergeObject == true)
	{
		if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		float x = atof(tokens[1].c_str());
		float y = atof(tokens[2].c_str());

		int ani_set_id = atoi(tokens[3].c_str());

		AnimationSets * animation_sets = AnimationSets::GetInstance();

		Box *obj = new Box();

		// General object setup
		obj->SetPosition(x, y);

		LPAnimation_Set ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<Boxs *>(objects[i]))
			{
				Boxs *boxs = dynamic_cast<Boxs *>(objects[i]);
				if (boxs->getMergeComplete() == false)
				{
					boxs->PushBox(obj);
				}
			}
		}
	}
	else
	{
		// DO NORMALLY

		if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		float x = atof(tokens[1].c_str());
		float y = atof(tokens[2].c_str());

		int ani_set_id = atoi(tokens[3].c_str());

		AnimationSets * animation_sets = AnimationSets::GetInstance();

		GameObject *obj = NULL;
		FireMario *bullet = NULL;

		switch (object_type)
		{
		case OBJECT_TYPE_MARIO:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				obj = player;
				//return;
			}
			else {
				obj = new Mario(x, y);
				player = (Mario*)obj;
			}

			DebugOut(L"[INFO] Player object created!\n");
			break;
		case OBJECT_TYPE_GOOMBA: obj = new Goomba(); break;
		case OBJECT_TYPE_BRICK: obj = new Brick(); break;
		case OBJECT_TYPE_GROUND: obj = new Ground(); break;
		case OBJECT_TYPE_BACKGROUND: obj = new BackGround(); break;
			/*case OBJECT_TYPE_FIRE_MARIO:
				obj = new FireMario();
				bullet = (FireMario*)obj;
				if (player != NULL)
				{
					player->SetBullet(bullet);
				}
				else {
					player = new Mario(50, 50);
					player->SetBullet(bullet);
				}
				break;*/
		case OBJECT_TYPE_KOOPA: obj = new Koopa(); break;
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}

		// General object setup
		obj->SetPosition(x, y);


		LPAnimation_Set ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
}
void PlayScene::Load()
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
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	Textures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void PlayScene::Update(DWORD dt)
{
	// We know that Mario is the last object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 



	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	if (player->GetShot() == true && player->GetLevel() == MARIO_LEVEL_FIRE && FireMario::count < FIRE_MARIO_MAX_ITEM)
	{
		GameObject *bullet = new FireMario();
		if (player->nx > 0)
		{
			bullet->SetState(FIRE_MARIO_STATE_RIGHT);
			bullet->SetPosition(player->x + MARIO_BIG_BBOX_WIDTH * 1.1f, player->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
		}
		else {
			bullet->SetState(FIRE_MARIO_STATE_LEFT);
			bullet->SetPosition(player->x - MARIO_BIG_BBOX_WIDTH * 0.1f, player->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
		}

		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(FIRE_MARIO_ANIMATION_SET_ID);

		bullet->SetAnimationSet(ani_set);
		objects.push_back(bullet);
		player->SetShot(false);
	}

	vector<LPGameObject> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->disable == false && dynamic_cast<Mario *>(objects[i]) == false)
		{
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		if (objects[i]->disable == true || (!dynamic_cast<Mario *>(objects[i]) && objects[i]->y > 300))
		{
			//LPGameObject obj = objects[i];
			objects.erase(objects.begin() + i);
			//delete obj;
			FireMario::count--;
		}
	}

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	Game *game = Game::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;


	// Keep mario not overcome screen
	if (player->x < 5) {
		player->x = 5;
	}
	else if (player->x > 2862) {
		player->x = 2862;
	}// Overcome end map

	// Mario in head map
	if (player->x < (game->GetScreenWidth() / 2)) {
		cx = 0;
	}
	else if (player->x > 2880 - (game->GetScreenWidth())) {
		cx = 2880 - (game->GetScreenWidth());
	}// Mario in tail map

	if (player->y <= MARIO_LIMIT_FLY + game->GetScreenHeight() / 2.2f)
		cy = MARIO_LIMIT_FLY;


	if ((player->y < game->GetScreenHeight() / 4 && player->GetFly() == true) || (player->y <= game->GetScreenHeight() / 4 && player->GetFall() == true))
		Game::GetInstance()->SetCamPosition(cx, cy /*cy*/);
	else Game::GetInstance()->SetCamPosition(cx, 0.0f /*cy*/);
}

void PlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void PlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void PlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario *mario = ((PlayScene*)scene)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
	{
		if (mario->GetFly() != true && mario->GetFall() != true)
			mario->SetState(MARIO_STATE_JUMP);
		else if (mario->GetFall() == true)
		{
			mario->SetRestrain(true);
		}
	}
	break;
	case DIK_P:
		mario->Reset();
		break;
	case DIK_Z:
		mario->UpLevel();
		break;
	case DIK_X:
		mario->DownLevel();
		break;
	case DIK_W:
		mario->SetState(MARIO_STATE_HIT);
		break;
	case DIK_S:
		mario->SetShot(true);
		mario->SetHolding(false);
		break;
	case DIK_D:
		mario->SetAttack(true);
		break;
	}
}
void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario *mario = ((PlayScene*)scene)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_IDLE);
		break;
	case DIK_Q:
		mario->SetHolding(false);
		break;
	case DIK_A:
		mario->SetFastSpeed(false);
		break;
	case DIK_SPACE:
	{
		if (mario->GetFly() == true)
		{
			mario->SetFall(true);
			mario->SetFly(false);
			mario->SetReadyFly(false);
		}
		if (mario->GetRestrain() == true)
		{
			mario->SetRestrain(false);
		}
	}
	case DIK_S:
		mario->SetShot(false);
		break;
	case DIK_D:
		mario->SetAttack(false);
		break;
	}

}

void PlaySceneKeyHandler::KeyState(BYTE *states)
{
	Game *game = Game::GetInstance();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_Q))
	{
		mario->SetHolding(true);
	}
	if (game->IsKeyDown(DIK_A))
	{
		if (mario->GetFly() == false)
			mario->SetFastSpeed(true);
	}
	if (game->IsKeyDown(DIK_SPACE) && mario->GetReadyFly() == true && mario->GetFall() == false)
	{
		mario->SetFly(true);
		mario->SetFastSpeed(false);
	}
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->nx < 0)
		{
			mario->SetState(MARIO_STATE_SITTING_LEFT);
		}
		else
		{
			mario->SetState(MARIO_STATE_SITTING_RIGHT);
		}
	}
	else
	{
		mario->SetState(MARIO_STATE_IDLE);
	}
}

