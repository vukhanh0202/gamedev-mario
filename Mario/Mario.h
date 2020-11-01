#pragma once
#include "GameObject.h"
#include "FireMario.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_WALKING_SPEED_FAST		0.15f 

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_SITTING_RIGHT	400
#define MARIO_STATE_SITTING_LEFT	500
#define MARIO_STATE_DIE				600

//#define MARIO_STATE_HOLD			700
//#define MARIO_STATE_UNHOLD			800
#define MARIO_STATE_HIT				900


#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT		3
#define MARIO_ANI_SUPER_BIG_IDLE_RIGHT	4
#define MARIO_ANI_SUPER_BIG_IDLE_LEFT	5

#define MARIO_ANI_BIG_WALKING_RIGHT				6
#define MARIO_ANI_BIG_WALKING_LEFT				7
#define MARIO_ANI_SMALL_WALKING_RIGHT			8
#define MARIO_ANI_SMALL_WALKING_LEFT			9
#define MARIO_ANI_SUPER_BIG_WALKING_RIGHT		10
#define MARIO_ANI_SUPER_BIG_WALKING_LEFT		11

#define MARIO_ANI_BIG_SITTING_RIGHT				12
#define MARIO_ANI_BIG_SITTING_LEFT				13
#define MARIO_ANI_SUPER_BIG_SITTING_RIGHT		14
#define MARIO_ANI_SUPER_BIG_SITTING_LEFT		15

#define MARIO_ANI_DIE		16

#define MARIO_ANI_SMALL_HOLD_RIGHT			17
#define MARIO_ANI_SMALL_HOLD_LEFT			18
#define MARIO_ANI_BIG_HOLD_RIGHT			19
#define MARIO_ANI_BIG_HOLD_LEFT				20
#define MARIO_ANI_SUPER_BIG_HOLD_RIGHT		21
#define MARIO_ANI_SUPER_BIG_HOLD_LEFT		22

#define MARIO_ANI_SMALL_HIT_RIGHT			23
#define MARIO_ANI_SMALL_HIT_LEFT			24
#define MARIO_ANI_BIG_HIT_RIGHT				25
#define MARIO_ANI_BIG_HIT_LEFT				26
#define MARIO_ANI_SUPER_BIG_HIT_RIGHT		27
#define MARIO_ANI_SUPER_BIG_HIT_LEFT		28

#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT			29
#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT			30
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT			31
#define MARIO_ANI_BIG_HOLD_IDLE_LEFT			32
#define MARIO_ANI_SUPER_BIG_IDLE_HOLD_RIGHT		33
#define MARIO_ANI_SUPER_BIG_IDLE_HOLD_LEFT		34

#define	MARIO_LEVEL_SMALL			1
#define	MARIO_LEVEL_BIG				2
#define	MARIO_LEVEL_SUPER_BIG		3

#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_BBOX_HEIGHT			27
#define MARIO_BIG_BBOX_HEIGHT_SITTING	18

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15


#define MARIO_SUPER_BIG_BBOX_WIDTH			21
#define MARIO_SUPER_BIG_BBOX_HEIGHT			28
#define MARIO_SUPER_BIG_BBOX_HEIGHT_SITTING	18

#define MARIO_UNTOUCHABLE_TIME 5000


class Mario : public GameObject
{
	int level;
	int untouchable;
	int ny; // Determine sit or not

	boolean hold; // Hold koopa
	boolean hit;
	boolean fast; // fast speed
	boolean shot; // shot bullet(fire)


	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	DWORD lastJumpTime;
	boolean isCollision; // Check object is collision?

	FireMario *bullet;

public:
	/*Mario() : GameObject()
	{
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}*/
	void setIsCollision(boolean flag) { isCollision = flag; }
	void setLastJumpTime(DWORD lastJumpTime) { this->lastJumpTime = lastJumpTime; }
	Mario(float x = 0.0f, float y = 0.0f);

	virtual void Update(DWORD dt, vector<LPGameObject> *colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void SetHolding(boolean holding) { this->hold = holding; };
	void SetFastSpeed(boolean fast) { this->fast = fast; };
	void SetShot(boolean shot) { this->shot = shot; };
	void SetBullet(FireMario *bullet) { this->bullet = bullet; }
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	void UpLevel();
	void DownLevel();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	boolean GetHolding() { return this->hold; }
	int GetLevel() { return this->level; }
};

