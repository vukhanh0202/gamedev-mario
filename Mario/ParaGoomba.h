#pragma once
#include "GameObject.h"

#define PARA_GOOMBA_WALKING_SPEED		0.05f;
#define PARA_GOOMBA_DIE_DEFLECT_SPEED	0.5f
#define PARA_GOOMBA_JUMP				0.35f

#define PARA_GOOMBA_BBOX_WIDTH_SWING	20
#define PARA_GOOMBA_BBOX_HEIGHT_SWING	24
#define PARA_GOOMBA_BBOX_WIDTH			16
#define PARA_GOOMBA_BBOX_HEIGHT			16
#define PARA_GOOMBA_BBOX_HEIGHT_DIE		9

#define PARA_GOOMBA_STATE_WALKING_WING	100
#define PARA_GOOMBA_STATE_WALKING		200
#define PARA_GOOMBA_STATE_DIE			300
#define PARA_GOOMBA_STATE_DIE_DISAPPER	400

#define PARA_GOOMBA_ANI_DIE				0
#define PARA_GOOMBA_ANI_WALKING			1
#define PARA_GOOMBA_ANI_WALKING_WING	2
#define PARA_GOOMBA_ANI_DIE_DISAPPER	3

#define PARA_GOOMBA_GRAVITY				0.0015f

#define PARA_GOOMBA_TIME_DISAPPEAR		500  //0.5s
#define PARA_GOOMBA_TIME_JUMP			1500  //1.5s

#define OBJECT_TYPE_PARA_GOOMBA	20
class ParaGoomba : public GameObject
{
	DWORD timeJump;
	DWORD timeDie;
public:
	ParaGoomba() {
		nx = -1;
		SetState(PARA_GOOMBA_STATE_WALKING_WING);
		timeJump = 0;
		timeDie = 0;
	};
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);
	void setTimeDie(DWORD timeDie) { this->timeDie = timeDie; }
	int GetTypeObject() {
		return OBJECT_TYPE_PARA_GOOMBA;
	}
};

