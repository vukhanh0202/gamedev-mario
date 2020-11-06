#pragma once
#include "GameObject.h"
#include "Utils.h"

#define BRICK_FIRE_MARIO_WIDTH		12
#define BRICK_FIRE_MARIO_HEIGHT		12

#define FIRE_MARIO_STATE_RIGHT		100
#define FIRE_MARIO_STATE_LEFT		200
#define FIRE_MARIO_STATE_DISABLE	300

#define FIRE_MARIO_ANI_RIGHT	0
#define FIRE_MARIO_ANI_LEFT		1

#define FIRE_MARIO_SPEED		0.1f

#define FIRE_MARIO_DEFLECT		0.2f
#define FIRE_MARIO_GRAVITY		0.1f

#define FIRE_MARIO_ANIMATION_SET_ID		161


#define FIRE_MARIO_MAX_ITEM	3




class FireMario : public GameObject
{

public:
	static int count;
	FireMario() { count++;}

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
};

