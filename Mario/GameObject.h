#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Animation.h"

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define OBJECT_TYPE_DEFAULT -100		// special texture to draw object bounding box

using namespace std;

class GameObject;
typedef GameObject * LPGameObject;


struct CollisionEvent;
typedef CollisionEvent * LPCollisionEvent;
struct CollisionEvent
{
	LPGameObject obj;
	float t, nx, ny;


	float dx, dy;		// *RELATIVE* movement distance between this object and obj
	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGameObject obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCollisionEvent &a, LPCollisionEvent &b)
	{
		return a->t < b->t;
	}
};


class GameObject
{
public:
	double x;
	double y;

	double dx;	// dx = vx*dt
	double dy;	// dy = vy*dt

	double vx;
	double vy;

	int nx;

	int state;

	boolean disable;

	DWORD dt;

	LPAnimation_Set animation_set;

public:

	void SetPosition(double x, double y) { this->x = x, this->y = y; }
	void SetSpeed(double vx, double vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(double &x, double &y) { x = this->x; y = this->y; }
	void GetSpeed(double &vx, double &vy) { vx = this->vx; vy = this->vy; }

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
	void FilterCollision(
		vector<LPCollisionEvent> &coEvents,
		vector<LPCollisionEvent> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny,
		float &rdx,
		float &rdy);

	void SetAnimationSet(LPAnimation_Set ani_set) { animation_set = ani_set; }

	void RenderBoundingBox();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual int GetDirection() { return nx; }
	virtual int GetTypeObject() { return OBJECT_TYPE_DEFAULT; }
	GameObject();
	~GameObject();
};

