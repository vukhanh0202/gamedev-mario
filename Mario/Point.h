#pragma once
#include "GameObject.h"
#include "Hud.h"

#define POINT_ANI_0		0
#define POINT_ANI_1		1
#define POINT_ANI_2		2
#define POINT_ANI_3		3
#define POINT_ANI_4		4
#define POINT_ANI_5		5
#define POINT_ANI_6		6
#define POINT_ANI_7		7
#define POINT_ANI_8		8
#define POINT_ANI_9		9

#define OBJECT_TYPE_POINT		17

class Point : public Hud
{
public:
	Point(double position_default_x, double position_default_y) :Hud(position_default_x, position_default_y) { state = 0; }

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_POINT;
	}
};

