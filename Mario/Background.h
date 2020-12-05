#pragma once
#include "GameObject.h"
#include <map>

#define OBJECT_TYPE_BACKGROUND	-1
#define OBJECT_TYPE_BACKGROUND_MOTION	9
#define OBJECT_TYPE_BACKGROUND_MOTION_TILE	10
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK	11
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR	12
#define OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER	13

#define BACKGROUND_SPEED		0.1f 
using namespace std;

class BackGround : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND;
	}
};

class BackGroundMotion : public GameObject
{
	map<int, BackGroundMotion*> arr;

public:
	BackGroundMotion() { disable = true; }
	void InsertArr(int key, BackGroundMotion* value) {
		this->arr.insert(std::pair<int, BackGroundMotion*>(key, value));
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void Show() { disable = false; };
	virtual void Hide() { disable = true; };
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND_MOTION;
	}
};

class BackGroundMotionTile : public BackGroundMotion
{
public:
	BackGroundMotionTile() { disable = false; }
	void Show() {
		disable = false;
		this->vy = -BACKGROUND_SPEED;
	}
	void Hide() {
		disable = true;
		this->vy = 0;
	}
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND_MOTION_TILE;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
};

class BackGroundMotionLogoBlack : public BackGroundMotion
{
public:
	void Show() {
		disable = false;
		this->vy = BACKGROUND_SPEED * 1.5;
	}
	void Hide() {
		disable = true;
		this->vy = 0;
	}
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
};

class BackGroundMotionLogoColor : public BackGroundMotion
{
public:
	
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
};

class BackGroundMotionLogoNumber : public BackGroundMotion
{
public:
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER;
	}
};

