#pragma once
#include "GameObject.h"

#define PIHANRA_BBOX_WIDTH  16
#define PIHANRA_BBOX_HEIGHT 32

#define OBJECT_TYPE_PIHANRA	22

#define PIHANRA_TIME_FIRE	500
#define PIHANRA_TIME_WAIT	1500

#define PIHANRA_SPEED		0.025f

class Pihanra : public GameObject
{
	int ny;
	double position_default_x;
	double position_default_y;
	boolean wait;
	DWORD lastFireTime;
	DWORD lastAppearTime;
public:
	virtual void Render();
	Pihanra(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
		vy = -PIHANRA_SPEED;
		ny = 1;
		wait = false;
		lastFireTime = 0;
		lastAppearTime = 0;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_PIHANRA;
	}
};
