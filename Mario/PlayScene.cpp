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
#include "Hud.h"
#include "Portal.h"
#include "Coin.h"
#include "Point.h"
#include "VenusFire.h"
#include "FireEnemy.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "Pihanra.h"
#include "Bonus.h"
#include "FallDrain.h"
#include "Mario.h"

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

#define OBJECT_TYPE_MARIO				-2
#define OBJECT_TYPE_BACKGROUND			-1
#define OBJECT_TYPE_GROUND				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPA				3
#define OBJECT_TYPE_FIRE_MARIO			4
#define OBJECT_TYPE_BOX					5
#define OBJECT_TYPE_BOXS				6
#define OBJECT_TYPE_HUD					7
#define OBJECT_TYPE_HUD_SPEED			8
#define OBJECT_TYPE_BACKGROUND_MOTION	9
#define OBJECT_TYPE_BACKGROUND_MOTION_TILE	10
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK	11
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR	12
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER	13
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_ARROW	14
#define OBJECT_TYPE_SWITCH_MAP	15
#define OBJECT_TYPE_COIN	16
#define OBJECT_TYPE_HUD_POINT	17
#define OBJECT_TYPE_VENUS_FIRE	18
#define OBJECT_TYPE_FIRE_ENEMY	19
#define OBJECT_TYPE_PARA_GOOMBA	20
#define OBJECT_TYPE_PARA_KOOPA	21
#define OBJECT_TYPE_PIHANRA	22
#define OBJECT_TYPE_BRICK_QUESTION_COIN	23
#define OBJECT_TYPE_BRICK_QUESTION_BONUS	24
#define OBJECT_TYPE_BONUS		25  // Mushroom red, leaf
#define OBJECT_TYPE_FALL_DRAIN_MAP_1_1		26  
#define OBJECT_TYPE_END_MAP_1_1				27 
#define OBJECT_TYPE_HUD_TIME				28
#define OBJECT_TYPE_HUD_SCORE				29


#define HUD_HEIGHT	53



#define OBJECT_TYPE_PORTAL	50

#define SCENE_MAP_SWITCH	2
#define SCENE_1				3

#define SPACE_ENOUGH_SWITCH_MAP		10
#define SCENE_1_COORDINATES_X		55
#define SCENE_1_COORDINATES_Y		0



#define HEIGHT_INVALIED		1000
#define CAM_UNDER_WORLD		255

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
						double xBoxs, yBoxs;
						listBox[0]->GetPosition(xBoxs, yBoxs);
						boxs->SetPosition(xBoxs, yBoxs);
						boxs->setWidth(BOX_BBOX_WIDTH);
						boxs->setHeight(BOX_BBOX_HEIGHT);


						for (size_t i = 1; i < listBox.size(); i++)
						{
							double xItem, yItem;
							listBox[i]->GetPosition(xItem, yItem);

							// Set width
							if (xItem >= xBoxs)
							{
								boxs->setWidth(xItem - xBoxs + BOX_BBOX_WIDTH);
							}
							else
							{
								double width = boxs->getWidth();
								boxs->setWidth(xBoxs - xItem + width);
								xBoxs = xItem;
							}

							// Set height
							if (yItem < yBoxs)
							{
								double height = boxs->getHeight();
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
	if (mergeObject)
	{
		if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

		int object_type = atoi(tokens[0].c_str());
		double x = atof(tokens[1].c_str());
		double y = atof(tokens[2].c_str());

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
		double x = atof(tokens[1].c_str());
		double y = atof(tokens[2].c_str());

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
				return;
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
		case OBJECT_TYPE_KOOPA: obj = new Koopa(); break;
		case OBJECT_TYPE_HUD: obj = new Hud(x, y); break;
		case OBJECT_TYPE_HUD_SPEED: obj = new HudSpeed(x, y); player->addHudSpeed((HudSpeed*)obj); break;
		case OBJECT_TYPE_BACKGROUND_MOTION:
			obj = new BackGroundMotion();
			this->bgMotion = (BackGroundMotion*)obj;
			break;
		case OBJECT_TYPE_BACKGROUND_MOTION_TILE:
			obj = new BackGroundMotionTile();
			this->bgMotion->InsertArr(OBJECT_TYPE_BACKGROUND_MOTION_TILE, (BackGroundMotionTile*)obj);
			break;
		case OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK:
			obj = new BackGroundMotionLogoBlack();
			this->bgMotion->InsertArr(OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK, (BackGroundMotionLogoBlack*)obj);
			break;
		case OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR:
			obj = new BackGroundMotionLogoColor();
			this->bgMotion->InsertArr(OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR, (BackGroundMotionLogoColor*)obj);
			break;
		case OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER:
			obj = new BackGroundMotionLogoNumber();
			this->bgMotion->InsertArr(OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER, (BackGroundMotionLogoNumber*)obj);
			break;
		case OBJECT_TYPE_BACKGROUND_MOTION_LOGO_ARROW:
			obj = new BackGroundMotionLogoArrow();
			this->bgMotion->InsertArrArrow((BackGroundMotionLogoArrow*)obj);
			break;
		case OBJECT_TYPE_SWITCH_MAP:
			obj = new Mario(x, y);
			player = (Mario*)obj;
			player->SetLevel(MARIO_LEVEL_SWITCH_MAP);
			player->SetSpeed(0, 0);
			break;
		case OBJECT_TYPE_COIN: obj = new Coin(); break;
		case OBJECT_TYPE_HUD_POINT: obj = new Point(x, y); player->addPoint((Point*)obj); break;
		case OBJECT_TYPE_HUD_TIME: obj = new Point(x, y); player->addTime((Point*)obj); break;
		case OBJECT_TYPE_HUD_SCORE: obj = new Point(x, y); player->addScore((Point*)obj); break;
		case OBJECT_TYPE_VENUS_FIRE: obj = new VenusFire(x, y); break;
		case OBJECT_TYPE_PARA_GOOMBA: obj = new ParaGoomba(); break;
		case OBJECT_TYPE_PARA_KOOPA: obj = new ParaKoopa(); break;
		case OBJECT_TYPE_PIHANRA: obj = new Pihanra(x, y); break;
		case OBJECT_TYPE_BRICK_QUESTION_COIN: obj = new BrickQuestionCoin(x,y); break;
		case OBJECT_TYPE_BRICK_QUESTION_BONUS: obj = new BrickQuestionBonus(x,y); break;
		case OBJECT_TYPE_FALL_DRAIN_MAP_1_1: obj = new FallDrainMap11(x, y); break;
		case OBJECT_TYPE_END_MAP_1_1: obj = new EndMap11(x, y); break;
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

	Game *game = Game::GetInstance();

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL || player->GetLevel() == MARIO_LEVEL_SWITCH_MAP) {

		vector<LPGameObject> coObjects;
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetTypeObject() != OBJECT_TYPE_BACKGROUND
				&& objects[i]->GetTypeObject() != OBJECT_TYPE_HUD)
			{
				coObjects.push_back(objects[i]);
			}
		}
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(dt, &coObjects);
		}
		if (player != NULL) {
			Game *game = Game::GetInstance();
			if (player->x < 5) {
				player->x = 5;
			}
			else if (player->x > game->GetScreenWidth() - MARIO_SWITCH_MAP_WIDTH) {
				player->x = game->GetScreenWidth() - MARIO_SWITCH_MAP_WIDTH;
			}
			else if (player->y < 5) {
				player->y = 5;
			}
			else if (player->y > game->GetScreenHeight() - MARIO_SWITCH_MAP_HEIGHT - HUD_HEIGHT) {
				player->y = game->GetScreenHeight() - MARIO_SWITCH_MAP_HEIGHT - HUD_HEIGHT;
			}
		}
		Game::GetInstance()->SetCamPosition((int)0, (int)0);
	}
	else {
		vector<LPGameObject> coObjects;
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->disable == false && objects[i]->y <= HEIGHT_INVALIED
				&& objects[i]->GetTypeObject() != OBJECT_TYPE_BACKGROUND
				&& objects[i]->GetTypeObject() != OBJECT_TYPE_HUD
				&& objects[i]->GetTypeObject() != OBJECT_TYPE_HUD_POINT
				)
			{
				coObjects.push_back(objects[i]);
			}
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->disable || (!dynamic_cast<Mario *>(objects[i]) && objects[i]->y > HEIGHT_INVALIED))
			{
				LPGameObject obj = objects[i];
				if (dynamic_cast<FireMario *>(objects[i])) {
					FireMario::count--;
					objects.erase(objects.begin() + i);
					delete obj;
				}
				else if (dynamic_cast<Coin *>(objects[i])) {
					objects.erase(objects.begin() + i);
					delete obj;
				}
				else if (dynamic_cast<FireEnemy *>(objects[i])) {
					objects.erase(objects.begin() + i);
					delete obj;
				}
				else if (dynamic_cast<Bonus *>(objects[i])) {
					objects.erase(objects.begin() + i);
					delete obj;
				}
			}
			else if (objects[i]->x >= player->x - game->GetScreenWidth() / 1.5 &&
				objects[i]->x <= player->x + game->GetScreenWidth() / 1.5 &&
				objects[i]->y >= player->y - game->GetScreenHeight()  &&
				objects[i]->y <= player->y + game->GetScreenHeight())
			{
				objects[i]->Update(dt, &coObjects);
			}
			else if (objects[i]->GetTypeObject() == OBJECT_TYPE_HUD_SPEED
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_HUD_BACKGROUND
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_HUD
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_HUD_POINT
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_GROUND
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_FIRE_MARIO
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_FALL_DRAIN_MAP_1_1
				|| objects[i]->GetTypeObject() == OBJECT_TYPE_END_MAP_1_1) {
				objects[i]->Update(dt, &coObjects);
			}
		}

		// Update camera to follow mario
		if (player != NULL) {
			double cx, cy;
			player->GetPosition(cx, cy);

			cx -= game->GetScreenWidth() / 2;
			cy -= game->GetScreenHeight() / 2;


			// Keep mario not overcome screen
			if (player->x < 5) {
				player->x = 5;
			}
			//else if (player->x > 2862) {
			//	player->x = 2862;
			//}// Overcome end map

			// Mario in head map
			if (player->x < (game->GetScreenWidth() / 2)) {
				cx = 0;
			}
			else if (player->x > END_MAP_1_1_POSITION_OUT_X - (game->GetScreenWidth() / 2)) {
				cx = (double)END_MAP_1_1_POSITION_OUT_X - (game->GetScreenWidth());
			}// Mario in tail map
			if (player->y < MARIO_LIMIT_FLY + (game->GetScreenHeight() / 1.2f))
				cy = MARIO_LIMIT_FLY;

			if ((player->y < game->GetScreenHeight() / 4 && player->GetFly())
				|| (player->y <= game->GetScreenHeight() / 4 && player->GetFall())
				|| (player->y <= -game->GetScreenHeight() / 4))
				Game::GetInstance()->SetCamPosition((int)cx, (int)cy);
			else if (player->y > UNDER_WORLD && player->GetState() != MARIO_STATE_DIE && player->getInTunnel()) {
				Game::GetInstance()->SetCamPosition((int)cx, (int)CAM_UNDER_WORLD );
			}
			else Game::GetInstance()->SetCamPosition((int)cx, (int)20.0f);
		}
		
	}
}

void PlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++) {
		if (!objects[i]->disable) {
			objects[i]->Render();
		}
	}
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

	if (mario == NULL) {
		BackGroundMotion *bg = ((PlayScene*)scene)->GetBGMotion();
		if (bg != NULL) {
			BackGroundMotionLogoArrow* logoArrow1 = (BackGroundMotionLogoArrow*)bg->GetArrArrow()[0];
			BackGroundMotionLogoArrow* logoArrow2 = (BackGroundMotionLogoArrow*)bg->GetArrArrow()[1];
			switch (KeyCode)
			{
			case DIK_DOWN:
				if (logoArrow1->disable) {
					logoArrow1->disable = false;
					logoArrow2->disable = true;
				}
				else {
					logoArrow1->disable = true;
					logoArrow2->disable = false;
				}
				break;
			case DIK_UP:
				if (logoArrow1->disable) {
					logoArrow1->disable = false;
					logoArrow2->disable = true;
				}
				else {
					logoArrow1->disable = true;
					logoArrow2->disable = false;
				}
				break;
			case DIK_A:
				if (!logoArrow1->disable) {
					Portal *p = new Portal(SCENE_MAP_SWITCH);
					Game::GetInstance()->SwitchScene(p->GetSceneId());
				}
				break;
			}
		}
	}
	else {
		if (!mario->getFallDrain() && !mario->getNoAction()) {
			if (mario->GetLevel() == MARIO_LEVEL_SWITCH_MAP) {
				switch (KeyCode)
				{
				case DIK_A:
					double x, y;
					mario->GetPosition(x, y);
					if (abs(x - SCENE_1_COORDINATES_X) < SPACE_ENOUGH_SWITCH_MAP && abs(y - SCENE_1_COORDINATES_Y) < SPACE_ENOUGH_SWITCH_MAP) {
						Portal *p = new Portal(SCENE_1);
						Game::GetInstance()->SwitchScene(p->GetSceneId());
					}
					break;
				}
			}
			else {
				switch (KeyCode)
				{
				case DIK_SPACE:
				{
					if (!mario->GetFly() && !mario->GetFall())
						mario->SetState(MARIO_STATE_JUMP);
					else if (mario->GetFall())
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
		}
	}
}
void PlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Mario *mario = ((PlayScene*)scene)->GetPlayer();
	if (mario == NULL) {}
	else {
		if (!mario->getFallDrain() && !mario->getNoAction()) {
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
				if (mario->GetFly())
				{
					mario->SetFall(true);
					mario->SetFly(false);
					mario->SetReadyFly(false);
				}
				if (mario->GetRestrain())
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
	}
}

void PlaySceneKeyHandler::KeyState(BYTE *states)
{
	Game *game = Game::GetInstance();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();
	if (mario == NULL) return;
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (!mario->getFallDrain() && !mario->getNoAction()) {
		if (game->IsKeyDown(DIK_Q))
		{
			mario->SetHolding(true);
		}
		if (game->IsKeyDown(DIK_A))
		{
			if (mario->GetFly() == false) {
				mario->SetFastSpeed(true);
			}
		}
		if (game->IsKeyDown(DIK_SPACE) && mario->GetReadyFly() && !mario->GetFall())
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
			if (mario->GetLevel() == MARIO_LEVEL_SWITCH_MAP) {
				mario->SetState(MARIO_STATE_WALKING_DOWN);
			}
			else {
				if (!mario->GetFly()) {
					if (mario->nx < 0)
					{
						mario->SetState(MARIO_STATE_SITTING_LEFT);
					}
					else
					{
						mario->SetState(MARIO_STATE_SITTING_RIGHT);
					}
				}
			}
		}
		else if (game->IsKeyDown(DIK_UP))
		{
			if (mario->GetLevel() == MARIO_LEVEL_SWITCH_MAP) {
				mario->SetState(MARIO_STATE_WALKING_UP);
			}
		}
		else
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
	}

}

