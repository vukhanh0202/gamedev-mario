#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_GRAVITY			0.1f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_RIGHT_FLASH	150
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_WALKING_LEFT_FLASH	250
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_JUMP_HIGH			350

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3


class Mario : public GameObject
{
	//int level;
	//int untouchable;
	//DWORD untouchable_start;
public:
	/*Mario() : GameObject()
	{
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}*/
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
	//void SetLevel(int l) { level = l; }
	//void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	//virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};

