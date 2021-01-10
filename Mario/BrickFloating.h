#pragma once
#include "GameObject.h"

#define BRICK_FLOATING_BBOX_WIDTH  48
#define BRICK_FLOATING_BBOX_HEIGHT 16

#define OBJECT_TYPE_BRICK_FLOATING	35

#define BRICK_FLOATING_WALKING_SPEED 0.03f
#define BRICK_FLOATING_FALL			0.00015f

#define BRICK_FLOATING_STATE_FLOAT	100
#define BRICK_FLOATING_STATE_FALL	200


class BrickFloating : public GameObject
{
public:
	BrickFloating();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK_FLOATING;
	}
};

