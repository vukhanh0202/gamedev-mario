#pragma once
#include "GameObject.h"
#include "Utils.h"

#define BOOMERANG_BBOX_WIDTH		12
#define BOOMERANG_BBOX_HEIGHT		12

#define BOOMERANG_STATE_LEFT		100
#define BOOMERANG_STATE_RIGHT		200
#define BOOMERANG_STATE_DISABLE		300

#define BOOMERANG_ANI_LEFT			0
#define BOOMERANG_ANI_RIGHT			1

#define BOOMERANG_SPEED				0.1f
#define BOOMERANG_SPEED_DEFLECT		0.07f

#define BOOMERANG_ANIMATION_SET_ID		32001

#define OBJECT_TYPE_BOOMERANG	43

#define MAX_DISTANCE_X			110
#define MAX_SPEED_Y				0.3f
class Boomerang: public GameObject
{
	double start_x;
	double start_y;
	int back;
public:
	int direct;
	Boomerang(double start_x, double start_y) {
		this->start_x = start_x;
		this->start_y = start_y;
		vy = 0;
		back = 0;
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);

	int GetTypeObject() {
		return OBJECT_TYPE_BOOMERANG;
	}
};

