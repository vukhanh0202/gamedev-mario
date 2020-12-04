#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_BACKGROUND	-1


class BackGround : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND;
	}
};

