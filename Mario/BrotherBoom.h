#pragma once
#include "GameObject.h"
#define BROTHER_BOOM_WALKING_SPEED		0.025f;
#define BROTHER_BOOM_DIE_DEFLECT_SPEED	0.1f


#define BROTHER_BOOM_BBOX_WIDTH			15
#define BROTHER_BOOM_BBOX_HEIGHT		24

#define BROTHER_BOOM_STATE_WALKING		100
#define BROTHER_BOOM_STATE_IDLE			200
#define BROTHER_BOOM_STATE_DIE			300

#define  BROTHER_BOOM_ANI_WALKING_LEFT			0
#define  BROTHER_BOOM_ANI_IDLE_LEFT				1
#define  BROTHER_BOOM_ANI_WALKING_RIGHT			2
#define  BROTHER_BOOM_ANI_IDLE_RIGHT			3
#define  BROTHER_BOOM_ANI_DIE					4

#define BROTHER_BOOM_GRAVITY				0.0015f

#define OBJECT_TYPE_BOOMERANG_BROTHER		42
#define DEPLAY_ATTACK					4000
#define DEPLAY_IDLE					1500

class BrotherBoom : public GameObject
{
	DWORD timeAttack;
	int delayFirstTime;
	DWORD timeIdle;
public:
	BrotherBoom();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);

	int GetTypeObject() {
		return OBJECT_TYPE_BOOMERANG_BROTHER;
	}
};

