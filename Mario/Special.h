#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_SPECIAL	32


#define SPECIAL_BBOX_WIDTH  16
#define SPECIAL_BBOX_HEIGHT 16

#define SPECIAL_STATE_NORMAL	100
#define SPECIAL_STATE_REWARD	200

#define SPECIAL_ANI_NORMAL		0
#define SPECIAL_ANI_REWARD		1



class Special : public GameObject
{

public:
	double position_default_x;
	double position_default_y;
	Special(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_SPECIAL;
	}
};

