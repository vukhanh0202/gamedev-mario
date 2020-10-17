#pragma once

#include "GameObject.h"

struct CollisionEvent;
typedef CollisionEvent * LPCollisionEvent;
struct CollisionEvent
{
	LPGameObject obj;
	float t, nx, ny;
	CollisionEvent(float t, float nx, float ny, LPGameObject obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCollisionEvent &a, LPCollisionEvent &b)
	{
		return a->t < b->t;
	}
};