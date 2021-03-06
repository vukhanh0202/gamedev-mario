#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_SPECIAL_STAR	32
#define OBJECT_TYPE_SPECIAL_FLOWER			38
#define OBJECT_TYPE_SPECIAL_MUSHROOM		39

#define SPECIAL_BBOX_WIDTH  16
#define SPECIAL_BBOX_HEIGHT 16

#define SPECIAL_STATE_NORMAL	100
#define SPECIAL_STATE_REWARD	200
#define SPECIAL_STATE_REWARD_COMPLETED	300

#define SPECIAL_ANI_NORMAL					0
#define SPECIAL_ANI_REWARD					1
#define SPECIAL_ANI_REWARD_COMPLETED		2

#define SPECIAL_ANI_SET_ID_STAR				25001
#define SPECIAL_ANI_SET_ID_FLOWER			25002
#define SPECIAL_ANI_SET_ID_MUSHROOM			25003

#define REWARD_1_X	194
#define REWARD_2_X	218
#define REWARD_3_X	242
#define REWARD_Y	193

#define PRIZE_X_1				2623
#define PRIZE_Y_1				20
#define PRIZE_X_1_SUB			2799
#define PRIZE_Y_1_SUB			69
#define PRIZE_X_2				2300
#define PRIZE_Y_2				20
#define PRIZE_X_2_SUB			2476
#define PRIZE_Y_2_SUB			69
#define PRIZE_ANI_SET_ID		99999

#define TIME_EXIST				5000				


class Special : public GameObject
{

public:
	int type;
	boolean status;
	DWORD last;
	double position_default_x;
	double position_default_y;
	Special(double position_default_x, double position_default_y) {
		this->position_default_x = position_default_x; this->position_default_y = position_default_y;
		status = false;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGameObject> *coObjects);
	int GetTypeObject() {
		return OBJECT_TYPE_SPECIAL_STAR;
	}
};

