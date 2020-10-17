#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Collision.h"
#include "Animation.h"

using namespace std;

typedef GameObject * LPGameObject;
class GameObject
{
//public:
	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	int state;

	DWORD dt;

	vector<LPAnimation> animations;

public:

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }


	/*
		Algorithm SweptAABB handling collision
		Extension of original SweptAABB to deal with two moving objects
	*/
	LPCollisionEvent SweptAABBEx(LPGameObject coO);

	/*
		Return list can happen collision
	*/
	void CalcPotentialCollisions(vector<LPGameObject> *coObjects, vector<LPCollisionEvent> &coEvents);

	/*
		Filter handling collision is prioritized
	*/
	void FilterCollision(vector<LPCollisionEvent> &coEvents, vector<LPCollisionEvent> &coEventsResult,
		float &min_tx, float &min_ty, float &nx, float &ny);

	/*
		Add animation for GameOject(1 GameOject can have some animations)
	*/
	void AddAnimation(int animationId);

	void RenderBoundingBox();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	GameObject();
	~GameObject();
};

