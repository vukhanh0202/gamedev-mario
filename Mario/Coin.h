#pragma once
#include "GameObject.h"


#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_EARN 100
#define COIN_STATE_TIMEOUT 200

#define OBJECT_TYPE_COIN		16
#define COIN_SPEED		0.5f
#define COIN_GRAVITY			0.002f

#define COIN_ANIMATION_SET_ID		11001
#define TIME_COIN_TIME_OUT		5000

#define BRICK_GLASS_ANI_SET_ID	131

class Coin : public GameObject
{
	double position_default_x;
	double position_default_y;
public:
	Coin():GameObject() {};
	DWORD timeAppear;
	Coin(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
		vy = -COIN_SPEED;
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_COIN;
	}
};
