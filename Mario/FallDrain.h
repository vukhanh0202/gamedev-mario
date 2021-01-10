#pragma once
#include "GameObject.h"
#include "Game.h"

#define OBJECT_TYPE_FALL_DRAIN_MAP_1_1		26  
#define OBJECT_TYPE_END_MAP_1_1				27 
#define OBJECT_TYPE_FALL_DRAIN_MAP_1_4		33

#define DRAIN_MAP_1_1_POSITION_IN_X			2170
#define DRAIN_MAP_1_1_POSITION_IN_Y			280
#define DRAIN_MAP_1_1_POSITION_OUT_X		2405
#define DRAIN_MAP_1_1_POSITION_OUT_Y		280 
#define DRAIN_MAP_1_1_POSITION_END_X		2397
#define DRAIN_MAP_1_1_POSITION_END_Y		160

#define DRAIN_MAP_1_4_POSITION_START_X		1930
#define DRAIN_MAP_1_4_POSITION_END_X		2185
#define DRAIN_MAP_1_4_POSITION_END_Y		158

#define END_MAP_1_1_POSITION_OUT_X			2896


#define DRAIN_BBOX_WIDTH  2
#define DRAIN_BBOX_HEIGHT 14

#define END_MAP_BBOX_HEIGHT 500


#define SPEED_SWAP_SCENE 0.02f

#define DISTANCE_SWAP		10
#define SCENE_MAP_SWITCH	2



class FallDrain : public GameObject
{
protected:
	double positionStartX, positionStartY;
	double positionInX, positionInY;
	double positionOutX, positionOutY;
	double positionEndX, positionEndY;

	boolean marioIn;
	boolean marioOut;
	boolean isEmpty;

public:
	FallDrain(double positionStartX, double positionStartY) {
		this->positionStartX = positionStartX; this->positionStartY = positionStartY;
		marioIn = marioOut = isEmpty = false;
	}
	void setStatus(boolean marioIn, boolean marioOut) { this->marioIn = marioIn; this->marioOut = marioOut; }
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

};

class EndMap11 : public FallDrain {
public:
	EndMap11(double positionStartX, double positionStartY) : FallDrain(positionStartX, positionStartY) {
		this->positionEndX = END_MAP_1_1_POSITION_OUT_X;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_END_MAP_1_1;
	}
};

class FallDrainMap11 : public FallDrain {
public:
	FallDrainMap11(double positionStartX, double positionStartY) : FallDrain (positionStartX,positionStartY) {
		this->positionInX = DRAIN_MAP_1_1_POSITION_IN_X;
		this->positionInY = DRAIN_MAP_1_1_POSITION_IN_Y;
		this->positionOutX = DRAIN_MAP_1_1_POSITION_OUT_X;
		this->positionOutY = DRAIN_MAP_1_1_POSITION_OUT_Y;
		this->positionEndX = DRAIN_MAP_1_1_POSITION_END_X;
		this->positionEndY = DRAIN_MAP_1_1_POSITION_END_Y;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_FALL_DRAIN_MAP_1_1;
	}
};

class FallDrainMap14 : public FallDrain {
public:
	FallDrainMap14(double positionStartX, double positionStartY) : FallDrain(positionStartX, positionStartY) {
		this->positionEndX = DRAIN_MAP_1_4_POSITION_END_X;
		this->positionEndY = DRAIN_MAP_1_4_POSITION_END_Y;
		this->positionStartX = DRAIN_MAP_1_4_POSITION_START_X;
	}
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_FALL_DRAIN_MAP_1_4;
	}
};

