#pragma once
#include "GameObject.h"

#define PARA_KOOPA_WALKING_SPEED 0.03f;
#define PARA_KOOPA_THROWING_SPEED 0.15f;
#define PARA_KOOPA_DIE_DEFLECT_SPEED	 0.4f
#define PARA_KOOPA_TIME_JUMP			1500  //1.5s
#define PARA_KOOPA_JUMP				0.35f



#define PARA_KOOPA_BBOX_WIDTH_WING 16
#define PARA_KOOPA_BBOX_HEIGHT_WING 27
#define PARA_KOOPA_BBOX_WIDTH 16
#define PARA_KOOPA_BBOX_HEIGHT 25
#define PARA_KOOPA_BBOX_HEIGHT_DIE 16

#define PARA_KOOPA_STATE_WALKING			100
#define PARA_KOOPA_STATE_DIE				200
#define PARA_KOOPA_STATE_THROWING_LEFT		300
#define PARA_KOOPA_STATE_THROWING_RIGHT		400
#define PARA_KOOPA_STATE_HOLDING			500
#define PARA_KOOPA_STATE_DIE_DISAPPER		600
#define PARA_KOOPA_STATE_WALKING_WING		700



#define PARA_KOOPA_ANI_WALKING_LEFT_WING		0
#define PARA_KOOPA_ANI_WALKING_RIGHT_WING		1
#define PARA_KOOPA_ANI_WALKING_LEFT				2
#define PARA_KOOPA_ANI_WALKING_RIGHT			3
#define PARA_KOOPA_ANI_DIE						4
#define PARA_KOOPA_ANI_THROWING					5
#define PARA_KOOPA_ANI_DIE_DISAPPEAR			6

#define PARA_KOOPA_GRAVITY			0.0015f

#define OBJECT_TYPE_PARA_KOOPA	21


class ParaKoopa : public GameObject
{
	DWORD timeJump;
public:
	DWORD timeDie;
	ParaKoopa();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);
	int GetTypeObject() {
		return OBJECT_TYPE_PARA_KOOPA;
	}
};


