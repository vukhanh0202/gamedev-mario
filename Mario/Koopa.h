#pragma once
#include "GameObject.h"

#define KOOPA_WALKING_SPEED 0.03f;
#define KOOPA_THROWING_SPEED 0.15f;
#define KOOPA_DIE_DEFLECT_SPEED	 0.4f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 25
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_STATE_WALKING			100
#define KOOPA_STATE_DIE				200
#define KOOPA_STATE_THROWING_LEFT	300
#define KOOPA_STATE_THROWING_RIGHT	400
#define KOOPA_STATE_HOLDING			500
#define KOOPA_STATE_DIE_DISAPPER	600

#define KOOPA_ANI_WALKING_LEFT		0
#define KOOPA_ANI_WALKING_RIGHT		1
#define KOOPA_ANI_DIE				2
#define KOOPA_ANI_THROWING			3
#define KOOPA_ANI_DIE_DISAPPEAR		4

//#define KOOPA_ANI_WALKING_LEFT_SWING		0
//#define KOOPA_ANI_WALKING_RIGHT_SWING		1
//#define KOOPA_ANI_WALKING_LEFT				2
//#define KOOPA_ANI_WALKING_RIGHT				3
//#define KOOPA_ANI_DIE						4
//#define KOOPA_ANI_THROWING					5
//#define KOOPA_ANI_DIE_DISAPPEAR				6

#define KOOPA_GRAVITY			0.0015f

#define OBJECT_TYPE_KOOPA	3

#define TIME_REVIVE			5000



class Koopa : public GameObject
{
	
public:
	DWORD timeDie;
	Koopa();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);
	int GetTypeObject() {
		return OBJECT_TYPE_KOOPA;
	}
};



