#pragma once
#include "GameObject.h"

#define KOOPA_WALKING_SPEED 0.03f;
#define KOOPA_THROWING_SPEED 0.15f;

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200
#define KOOPA_STATE_THROWING_LEFT 300
#define KOOPA_STATE_THROWING_RIGHT 400
#define KOOPA_STATE_HOLDING 500

#define KOOPA_ANI_WALKING_LEFT 0
#define KOOPA_ANI_WALKING_RIGHT 1
#define KOOPA_ANI_DIE 2
#define KOOPA_ANI_THROWING 3

#define KOOPA_GRAVITY			0.005f


class Koopa : public GameObject
{
	
public:
	Koopa();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
};

