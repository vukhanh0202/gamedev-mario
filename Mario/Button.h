#pragma once
#include "GameObject.h"


#define BUTTON_BBOX_WIDTH  16
#define BUTTON_BBOX_HEIGHT 16
#define BUTTON_BBOX_HEIGHT_PRESS 6


#define BUTTON_STATE_NORMAL		100
#define BUTTON_STATE_PRESS		200

#define BUTTON_ANI_NORMAL		0
#define BUTTON_ANI_PRESS		1

#define BUTTON_ANIMATION_SET_ID		21001

class Button : public GameObject
{
	double position_default_x;
	double position_default_y;
public:
	Button(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
		vy = -0.00005f;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);

	int GetTypeObject() {
		return 0;
	}
};
