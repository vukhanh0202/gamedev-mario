#pragma once
#include "GameObject.h"
#include "FireMario.h"
#include "Koopa.h"
#include "Hud.h"
#include "Point.h"
#include <algorithm>
#include "Button.h"
#include "Special.h"


#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_WALKING_SPEED_FAST		0.15f 

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_FLY				0.00005f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_SITTING_RIGHT	400
#define MARIO_STATE_SITTING_LEFT	500
#define MARIO_STATE_DIE				600
#define MARIO_STATE_WALKING_DOWN	700
#define MARIO_STATE_WALKING_UP		800

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
#define MARIO_ANI_SUPER_BIG_FLY_RIGHT			35
#define MARIO_ANI_SUPER_BIG_FLY_LEFT			36
#define MARIO_ANI_SUPER_BIG_FALL_RIGHT			37
#define MARIO_ANI_SUPER_BIG_FALL_LEFT			38
#define MARIO_ANI_SUPER_BIG_RESTRAIN_RIGHT		39
#define MARIO_ANI_SUPER_BIG_RESTRAIN_LEFT		40
#define MARIO_ANI_SUPER_BIG_READY_FLY_RIGHT		41
#define MARIO_ANI_SUPER_BIG_READY_FLY_LEFT		42

#define MARIO_ANI_FIRE_IDLE_RIGHT				43
#define MARIO_ANI_FIRE_IDLE_LEFT				44
#define MARIO_ANI_FIRE_WALKING_RIGHT			45
#define MARIO_ANI_FIRE_WALKING_LEFT				46
#define MARIO_ANI_FIRE_SITTING_RIGHT			47
#define MARIO_ANI_FIRE_SITTING_LEFT				48
#define MARIO_ANI_FIRE_HOLD_RIGHT				49
#define MARIO_ANI_FIRE_HOLD_LEFT				50
#define MARIO_ANI_FIRE_HIT_RIGHT				51
#define MARIO_ANI_FIRE_HIT_LEFT					52
#define MARIO_ANI_FIRE_HOLD_IDLE_RIGHT			53
#define MARIO_ANI_FIRE_HOLD_IDLE_LEFT			54
#define MARIO_ANI_FIRE_SHOT_RIGHT				55
#define MARIO_ANI_FIRE_SHOT_LEFT				56

#define MARIO_ANI_SUPER_BIG_ATTACK_RIGHT		57
#define MARIO_ANI_SUPER_BIG_ATTACK_LEFT			58

#define MARIO_ANI_BIG_FALL_DRAIN				59
#define MARIO_ANI_SMALL_FALL_DRAIN				60
#define MARIO_ANI_SUPER_BIG_FALL_DRAIN			61
#define MARIO_ANI_FIRE_FALL_DRAIN				62

#define	MARIO_LEVEL_SWITCH_MAP		0
#define	MARIO_LEVEL_SMALL			1
#define	MARIO_LEVEL_BIG				2
#define	MARIO_LEVEL_FIRE			3
#define	MARIO_LEVEL_SUPER_BIG		4


#define MARIO_BIG_BBOX_WIDTH			15
#define MARIO_BIG_BBOX_WIDTH_HOLDING	29
#define MARIO_BIG_BBOX_HEIGHT			27
#define MARIO_BIG_BBOX_HEIGHT_SITTING	18

#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_WIDTH_HOLDING  27
#define MARIO_SMALL_BBOX_HEIGHT			15


#define MARIO_SUPER_BIG_BBOX_WIDTH			19
#define MARIO_SUPER_BIG_BBOX_WIDTH_HOLDING	35
#define MARIO_SUPER_BIG_BBOX_HEIGHT			28
#define MARIO_SUPER_BIG_BBOX_HEIGHT_SITTING	18

#define MARIO_FIRE_BBOX_WIDTH			16
#define MARIO_FIRE_BBOX_WIDTH_HOLDING	30
#define MARIO_FIRE_BBOX_HEIGHT			27
#define MARIO_FIRE_BBOX_HEIGHT_SITTING	18

#define MARIO_SWITCH_MAP_WIDTH			16
#define MARIO_SWITCH_MAP_HEIGHT			19

#define MARIO_UNTOUCHABLE_TIME 5000

#define MARIO_POWER_READY_FLY	240

#define MARIO_LIMIT_FLY			-210

#define OBJECT_TYPE_MARIO	-2

#define UNDER_WORLD		220

#define TIME_PLAY		300
#define SCORE_PLUS		100
#define TIME_PER_ONE_ATTACK		400



class Mario : public GameObject
{
	int level;
	int untouchable;
	int ny; // Determine sit or not
	int power; // Determine Mario ready for fly (power = 5 mario can fly)
	int point; // point when mario earn gold
	int time; // point when mario earn gold
	int countTime;
	int score;

	boolean hold; // Hold koopa
	boolean hit;
	boolean fast; // fast speed
	boolean shot; // shot bullet(fire)
	boolean readyFly; //Determine mario can fly or not
	boolean fly;  // Determine mario fly or not
	boolean fall; // Determine mario fall. While falling, mario can not fly
	boolean restrain; // Restrain mario fall
	boolean attack; // Mario swing attack

	boolean fallDrain;
	boolean noAction;
	boolean inTunnel;

	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	DWORD lastJumpTime;
	DWORD lastAttack;
	boolean isCollision; // Check object is collision?

	//FireMario *bullet;
	GameObject *koopaHold;


	vector<HudSpeed*> hudSpeedList;

	vector<Point*> hudPointList;
	vector<Point*> hudTimeList;
	vector<Point*> hudScoreList;

public:
	Special *reward;
	boolean isCollisionKoopa;
	boolean bonusInMap;
	/*Mario() : GameObject()
	{
		level = MARIO_LEVEL_BIG;
		untouchable = 0;
	}*/
	void setIsCollision(boolean flag) { isCollision = flag; }
	void setLastJumpTime(DWORD lastJumpTime) { this->lastJumpTime = lastJumpTime; }
	Mario(float x = 0.0f, float y = 0.0f);

	int getUntouchable() { return this->untouchable; }
	virtual void Update(DWORD dt, vector<LPGameObject> *colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void SetHolding(boolean holding) { this->hold = holding; };
	void SetFastSpeed(boolean fast) { this->fast = fast; };
	void SetShot(boolean shot) { this->shot = shot; };
	boolean GetShot() { return this->shot; }
	void SetFly(boolean fly) { this->fly = fly; }
	boolean GetFly() { return this->fly; }
	void SetFall(boolean fall) { this->fall = fall; }
	boolean GetFall() { return this->fall; }
	void SetReadyFly(boolean readyFly) { this->readyFly = readyFly; }
	boolean GetReadyFly() { return this->readyFly; }
	void SetRestrain(boolean restrain) { this->restrain = restrain; }
	boolean GetRestrain() { return this->restrain; }
	void SetAttack(boolean attack) { this->attack = attack; }
	boolean getAttack() { return this->attack; }
	void SetLevel(int l) { level = l; unableReadyFly(); }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void addHudSpeed(HudSpeed* hudSpeed) { this->hudSpeedList.push_back(hudSpeed); }
	void addPoint(Point* point) { this->hudPointList.push_back(point); }
	void addTime(Point* time) { this->hudTimeList.push_back(time); }
	void addScore(Point* score) { this->hudScoreList.push_back(score); }
	boolean getFallDrain() { return this->fallDrain; }
	void setFallDrain(boolean fallDrain) { this->fallDrain = fallDrain; }
	boolean getInTunnel() { return this->inTunnel; }
	void setInTunnel(boolean inTunnel) { this->inTunnel = inTunnel; }
	int getScore() { return this->score; }
	void setScore(int score) { this->score = score; }
	void setLastAttack(DWORD lastAttack) { this->lastAttack = lastAttack; }

	boolean getNoAction() { return this->noAction; }
	void setNoAction(boolean noAction) { this->noAction = noAction; }
	void unableReadyFly();
	int GetTypeObject() {
		return OBJECT_TYPE_MARIO;
	}

	void FilterCollision(
		vector<LPCollisionEvent> &coEvents,
		vector<LPCollisionEvent> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny,
		float &rdx,
		float &rdy);

	void Reset();
	void UpLevel();
	void DownLevel();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	boolean GetHolding() { return this->hold; }
	int GetLevel() { return this->level; }
	int GetHeightDrainFall();
};

