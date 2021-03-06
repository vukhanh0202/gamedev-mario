#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED		0.05f;
#define GOOMBA_DIE_DEFLECT_SPEED	0.5f


#define GOOMBA_BBOX_WIDTH			16
#define GOOMBA_BBOX_HEIGHT			14
#define GOOMBA_BBOX_HEIGHT_DIE		7

#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DIE_DISAPPER	300

#define GOOMBA_ANI_WALKING			0
#define GOOMBA_ANI_DIE				1
#define GOOMBA_ANI_DIE_DISAPPER		2

#define GOOMBA_GRAVITY				0.0015f

#define GOOMBA_TIME_DISAPPEAR		500  //0.5s


#define OBJECT_TYPE_GOOMBA		2

class Goomba : public GameObject
{
	DWORD timeDie;
public:
	Goomba();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);

	void setTimeDie(DWORD timeDie) { this->timeDie = timeDie; }
	int GetTypeObject() {
		return OBJECT_TYPE_GOOMBA;
	}
};


