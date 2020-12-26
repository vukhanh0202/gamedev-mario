#pragma once
#include "GameObject.h"
#include "Coin.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16


#define BRICK_STATE_NORMAL	100
#define BRICK_STATE_BROKEN	200

#define BRICK_ANI_NORMAL	0
#define BRICK_ANI_BROKEN	1



#define OBJECT_TYPE_BRICK_GLASS				30


class BrickGlass : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK_GLASS;
	}
};


