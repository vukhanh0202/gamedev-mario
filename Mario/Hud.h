#pragma once
#include "GameObject.h"




#define OBJECT_TYPE_HUD		7
#define OBJECT_TYPE_HUD_BACKGROUND		8
#define OBJECT_TYPE_HUD_SPEED		9

#define POSITION_DEFAULT	50
#define HUD_BOXX_HEIGHT		20
#define MAX_SPEED_QUANTITY	7

#define END_MAP_1_1_POSITION_OUT_X			2896


class Hud : public GameObject
{
protected:
	double position_default_x;
	double position_default_y;
public:
	Hud(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_HUD;
	}
};

class HudSpeed : public Hud {
public:
	HudSpeed(double position_default_x, double position_default_y) :Hud(position_default_x, position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
	}
};

