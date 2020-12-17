#pragma once
#include "GameObject.h"

#define VENUS_FIRE_BBOX_WIDTH  16
#define VENUS_FIRE_BBOX_HEIGHT 32

#define OBJECT_TYPE_VENUS_FIRE	18

#define VENUS_FIRE_ANI_LOW_LEFT		0
#define VENUS_FIRE_ANI_HIGH_LEFT	1
#define VENUS_FIRE_ANI_LOW_RIGHT	2
#define VENUS_FIRE_ANI_HIGH_RIGHT	3

#define VENUS_FIRE_ANI_LOW_LEFT_FIRE	4
#define VENUS_FIRE_ANI_HIGH_LEFT_FIRE	5
#define VENUS_FIRE_ANI_LOW_RIGHT_FIRE	6
#define VENUS_FIRE_ANI_HIGH_RIGHT_FIRE	7

#define VENUS_FIRE_TIME_FIRE	500
#define VENUS_FIRE_TIME_WAIT	1500

#define VENUS_FIRE_SPEED		0.025f

class VenusFire : public GameObject
{
	int ny;
	double position_default_x;
	double position_default_y;
	boolean fire;
	DWORD lastFireTime;
	DWORD lastAppearTime;
public:
	virtual void Render();
	VenusFire(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
		vy = -VENUS_FIRE_SPEED;
		ny = 1;
		fire = false;
		lastFireTime = 0;
		lastAppearTime = 0;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);
	int GetTypeObject() {
		return OBJECT_TYPE_VENUS_FIRE;
	}
};

