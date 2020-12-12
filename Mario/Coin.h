#pragma once
#include "GameObject.h"

#define COIN_STATE_ENABLE 100
#define COIN_STATE_DISABLE 200

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16

#define OBJECT_TYPE_COIN		16

class Coin : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_COIN;
	}
};
