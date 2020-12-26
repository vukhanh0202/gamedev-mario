#pragma once
#include "GameObject.h"
#include "BrickGlass.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16


#define BRICK_STATE_NORMAL	100
#define BRICK_STATE_EMPTY	200

#define BRICK_ANI_NORMAL	0
#define BRICK_ANI_EMPTY		1

#define PARA_GOOMBA_STATE_DIE_DISAPPER 100
#define PARA_GOOMBA_STATE_DIE_DISAPPER 200

#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_BRICK_QUESTION_COIN	23
#define OBJECT_TYPE_BRICK_QUESTION_BONUS 24

#define BRICK_DEFLECT_SPEED 0.1f

class Brick : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK;
	}
};
class BrickQuestion : public Brick
{
protected:
	boolean isEmpty;
	double positionDefaultX, positionDefaultY;
public:
	BrickQuestion(double positionDefaultX, double positionDefaultY) : Brick() {
		isEmpty = false;
		this->positionDefaultX = positionDefaultX;
		this->positionDefaultY = positionDefaultY;
	}
	virtual void Render();
};

class BrickQuestionCoin : public BrickQuestion
{
public:
	BrickQuestionCoin(double positionDefaultX, double positionDefaultY) : BrickQuestion(positionDefaultX, positionDefaultY) {
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK_QUESTION_COIN;
	}
};

class BrickQuestionBonus : public BrickQuestion
{
public:
	BrickQuestionBonus(double positionDefaultX, double positionDefaultY) : BrickQuestion(positionDefaultX, positionDefaultY) {
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK_QUESTION_BONUS;
	}
};

class BrickButton : public BrickQuestion
{
public:
	BrickButton(double positionDefaultX, double positionDefaultY) : BrickQuestion(positionDefaultX, positionDefaultY) {
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_BRICK_QUESTION_BONUS;
	}
};