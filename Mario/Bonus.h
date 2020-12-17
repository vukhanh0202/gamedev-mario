#pragma once
#include "GameObject.h"

#define BONUS_BBOX_WIDTH  16
#define BONUS_BBOX_HEIGHT 16

#define BONUS_STATE_MUSHROOM_RED 100
#define BONUS_STATE_LEAF		200

#define BONUS_ANI_MUSHROOM_RED	0
#define BONUS_ANI_LEAF			1

#define BONUS_SPEED		0.03f
#define BONUS_GRAVITY			0.001f
#define BONUS_GRAVITY_LEAF		0.0002f
#define BONUS_SPEED_LEAF		0.0015f

#define BONUS_TIME_REVERSE		350;

#define BONUS_ANIMATION_SET_ID		17001
#define OBJECT_TYPE_BONUS		25  // Mushroom red, leaf

class Bonus : public GameObject
{
	DWORD lastReverseSpeed;
public:
	Bonus() :GameObject() { vx = BONUS_SPEED_LEAF; lastReverseSpeed = 0; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BONUS;
	}
};

