#pragma once
#include "GameObject.h"
#include "Utils.h"

#define BRICK_FIRE_ENEMY_WIDTH		12
#define BRICK_FIRE_ENEMY_HEIGHT		12

#define FIRE_ENEMY_STATE_RIGHT		100
#define FIRE_ENEMY_STATE_LEFT		200
#define FIRE_ENEMY_STATE_DISABLE	300

#define FIRE_ENEMY_ANI_RIGHT	0
#define FIRE_ENEMY_ANI_LEFT		1

#define FIRE_ENEMY_SPEED		0.05f


#define FIRE_ENEMY_ANIMATION_SET_ID		161

#define OBJECT_TYPE_FIRE_ENEMY	19

#define TIME_FIRE_ENEMY_EXIST	3000


class FireEnemy : public GameObject
{
	DWORD timeAppear;
public:
	FireEnemy() { timeAppear = GetTickCount64(); }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);

	int GetTypeObject() {
		return OBJECT_TYPE_FIRE_ENEMY;
	}
};



