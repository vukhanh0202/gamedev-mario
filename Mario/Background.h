#pragma once
#include "GameObject.h"


class BackGround : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};

