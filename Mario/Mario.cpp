#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"
#include "Box.h"
#include "Koopa.h"
#include "PlayScene.h"
#include "Game.h"
#include "Utils.h"
#include "Coin.h"
#include "FireEnemy.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "VenusFire.h"
#include "Pihanra.h"

Mario::Mario(float x, float y) : GameObject()
{
	level = MARIO_LEVEL_SUPER_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	vy = 0.1f;

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	lastJumpTime = 0;
	ny = -1;
	hold = false;
	hit = false;
	shot = false;
	fly = false;
	attack = false;
	power = 0;
	//bullet = NULL;
	koopaHold = NULL;
	point = 0;
}

void Mario::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	if (level == MARIO_LEVEL_SUPER_BIG && hold == false)
	{
		// Top high => Mario fall
		if (this->y < MARIO_LIMIT_FLY)
		{
			fall = true;
			fly = false;
			readyFly = false;
		}

		// Mario accumulate to fly
		if (fast == true && readyFly == false && vx != 0)
		{
			this->power++;
			if (power >= MARIO_POWER_READY_FLY) readyFly = true;
		}
		else {
			if (power <= 0) readyFly = false;
			else power--;
		}

		// ----- Set speed hud
		int powerPerLevel = MARIO_POWER_READY_FLY / MAX_SPEED_QUANTITY;
		int levelFly = power / powerPerLevel;
		for (size_t i = 0; i < hudSpeedList.size(); i++)
		{
			if (i < levelFly || levelFly == MAX_SPEED_QUANTITY) {
				hudSpeedList[i]->disable = false;
			}
			else if (levelFly != MAX_SPEED_QUANTITY && readyFly == false) {
				hudSpeedList[i]->disable = true;
			}
		}
		// ----- End Set Speed hud

		// Speed fly
		if (fly == true)
		{
			vy = -0.2f;
		}
		else if (fall == true)
		{
			if (restrain == true)
			{
				vy = 0.1f;
			}
			else {
				vy = 0.2f;
			}
		}
		else {
			if (vy > 0 && y < 0 && GetTickCount() - lastJumpTime >500) fall = true;
			vy += MARIO_GRAVITY * dt;
		}
	}
	else if (level != MARIO_LEVEL_SWITCH_MAP) {
		vy += MARIO_GRAVITY * dt;
	}


	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// Handle mario hold koopa
	if (hold == true && koopaHold != NULL)
	{
		if (this->nx > 0)
		{
			if (this->level == MARIO_LEVEL_FIRE)
			{
				koopaHold->SetPosition(x + MARIO_FIRE_BBOX_WIDTH - 2, y + MARIO_FIRE_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_SUPER_BIG)
			{
				koopaHold->SetPosition(x + MARIO_SUPER_BIG_BBOX_WIDTH - 2, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_BIG)
			{
				koopaHold->SetPosition(x + MARIO_BIG_BBOX_WIDTH - 2, y + MARIO_BIG_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_SMALL)
			{
				koopaHold->SetPosition(x + MARIO_SMALL_BBOX_WIDTH - 2, y - 2);
			}
		}
		else if (this->nx < 0)
		{
			if (this->level == MARIO_LEVEL_FIRE)
			{
				koopaHold->SetPosition(x - KOOPA_BBOX_WIDTH + 3, y + MARIO_FIRE_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_SUPER_BIG)
			{
				koopaHold->SetPosition(x - KOOPA_BBOX_WIDTH + 3, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_BIG)
			{
				koopaHold->SetPosition(x - KOOPA_BBOX_WIDTH + 3, y + MARIO_BIG_BBOX_HEIGHT / 4);
			}
			else if (this->level == MARIO_LEVEL_SMALL)
			{
				koopaHold->SetPosition(x - KOOPA_BBOX_WIDTH + 3, y - 2);
			}
		}
	}
	else if (hold == false && koopaHold != NULL)
	{

		if (this->nx > 0)
		{
			//this->SetState(MARIO_STATE_HIT);
			koopaHold->SetState(KOOPA_STATE_THROWING_RIGHT);
			koopaHold = NULL;
		}
		else if (this->nx < 0)
		{
			//this->SetState(MARIO_STATE_HIT);
			koopaHold->SetState(KOOPA_STATE_THROWING_LEFT);
			koopaHold = NULL;
		}
	}

	// Handle mario shot bullet
	if (shot && level == MARIO_LEVEL_FIRE && FireMario::count < FIRE_MARIO_MAX_ITEM)
	{
		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();

		GameObject *bullet = new FireMario();
		if (this->nx > 0)
		{
			bullet->SetState(FIRE_MARIO_STATE_RIGHT);
			bullet->SetPosition(this->x + MARIO_BIG_BBOX_WIDTH * 1.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
		}
		else {
			bullet->SetState(FIRE_MARIO_STATE_LEFT);
			bullet->SetPosition(this->x - MARIO_BIG_BBOX_WIDTH * 0.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
		}

		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(FIRE_MARIO_ANIMATION_SET_ID);

		bullet->SetAnimationSet(ani_set);
		((PlayScene*)scene)->pushObject(bullet);
		SetShot(false);
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		/*if (fall == false && vy > 0)
		{
			fall = true;
		}*/
	}
	else
	{
		isCollision = true;
		fall = false;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (nx != 0) vx = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (dynamic_cast<Boxs *>(e->obj))
			{
				if (e->ny > 0)
				{
					y += dy;
				}
				else if (e->nx != 0)
				{
					x += dx;
				}
				else {
					if (ny != 0) vy = 0;
				}
			}
			else if (dynamic_cast<Coin *>(e->obj))
			{
				Coin *coin = dynamic_cast<Coin *>(e->obj);
				coin->disable = true;
				point++;
				if (point >= 100) point = 0;
				hudPointList.at(0)->SetState(point % 10);
				hudPointList.at(1)->SetState(point / 10);
			}
			else {
				if (ny != 0) vy = 0;
			}
			// Handle mario attack monster
			if (attack == true)
			{
				if (dynamic_cast<Goomba *>(e->obj)) // if e->obj is Goomba 
				{
					Goomba *goomba = dynamic_cast<Goomba *>(e->obj);

					if (goomba->state == GOOMBA_STATE_WALKING) {
						if (this->nx > 0) {
							goomba->nx = 1;
						}
						else {
							goomba->nx = -1;
						}
						goomba->state = GOOMBA_STATE_DIE_DISAPPER;
					}
				}
				else if (dynamic_cast<Koopa *>(e->obj)) // if e->obj is Koopa 
				{
					Koopa *koopa = dynamic_cast<Koopa *>(e->obj);

					if (this->nx > 0) {
						koopa->nx = 1;
					}
					else {
						koopa->nx = -1;
					}
					koopa->state = KOOPA_STATE_DIE_DISAPPER;
				}
				else if (dynamic_cast<ParaGoomba *>(e->obj)) // if e->obj is ParaGoomba 
				{
					ParaGoomba *paraGoomba = dynamic_cast<ParaGoomba *>(e->obj);

					if (this->nx > 0) {
						paraGoomba->nx = 1;
					}
					else {
						paraGoomba->nx = -1;
					}
					paraGoomba->state = PARA_GOOMBA_STATE_DIE_DISAPPER;
				}
				else if (dynamic_cast<ParaKoopa *>(e->obj)) // if e->obj is ParaKoopa 
				{
					ParaKoopa *paraKoopa = dynamic_cast<ParaKoopa *>(e->obj);

					if (this->nx > 0) {
						paraKoopa->nx = 1;
					}
					else {
						paraKoopa->nx = -1;
					}
					paraKoopa->state = PARA_KOOPA_STATE_DIE_DISAPPER;
				}
				else if (dynamic_cast<VenusFire *>(e->obj)) {
					VenusFire *venusFire = dynamic_cast<VenusFire *>(e->obj);

					venusFire->disable = true;
				}
				else if (dynamic_cast<Pihanra *>(e->obj)) {
					Pihanra *pihanra = dynamic_cast<Pihanra *>(e->obj);

					pihanra->disable = true;
				}
			}
			else
			{
				if (dynamic_cast<Goomba *>(e->obj)) // if e->obj is Goomba 
				{
					Goomba *goomba = dynamic_cast<Goomba *>(e->obj);

					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							goomba->setTimeDie(GetTickCount());
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									//level = MARIO_LEVEL_SMALL;
									level--;
									unableReadyFly();
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else if (dynamic_cast<Koopa *>(e->obj)) // if e->obj is Koopa 
				{
					Koopa *koopa = dynamic_cast<Koopa *>(e->obj);

					if (koopa->state != KOOPA_STATE_DIE_DISAPPER)
					{
						// jump on top >> kill Koopa
						if (e->ny < 0)
						{
							if (koopa->GetState() != KOOPA_STATE_DIE)
							{
								koopa->SetState(KOOPA_STATE_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
						}
						else if (e->nx > 0) // collision right to left
						{
							// Koopa die
							if (koopa->GetState() == KOOPA_STATE_DIE)
							{
								// Hold koopa
								if (this->hold == hold)
								{
									koopa->SetState(KOOPA_STATE_HOLDING);
									this->koopaHold = koopa;
								}
								else
								{
									// Hit Koopa
									//this->SetState(MARIO_STATE_HIT);
									koopa->SetState(KOOPA_STATE_THROWING_LEFT);
								}
							}
							else
							{
								// koopa live
								if (untouchable == 0)
								{
									if (level > MARIO_LEVEL_SMALL)
									{
										level--;
										unableReadyFly();
										StartUntouchable();
									}
									else
										SetState(MARIO_STATE_DIE);
								}
							}
						}
						else if (e->nx < 0) // collision left->right
						{
							if (koopa->GetState() == KOOPA_STATE_DIE)
							{
								// Hold koopa
								if (this->hold == true)
								{
									koopa->SetState(KOOPA_STATE_HOLDING);
									this->koopaHold = koopa;
								}
								else
								{
									//this->SetState(MARIO_STATE_HIT);
									koopa->SetState(KOOPA_STATE_THROWING_RIGHT);
								}

							}
							else
							{
								if (untouchable == 0)
								{
									if (level > MARIO_LEVEL_SMALL)
									{
										level--;
										unableReadyFly();
										StartUntouchable();
									}
									else
										SetState(MARIO_STATE_DIE);
								}
							}
						}
					}
				}
				else if (dynamic_cast<FireEnemy *>(e->obj)) // if e->obj is fireEnemy
				{
					// koopa live
					if (untouchable == 0)
					{
						if (level > MARIO_LEVEL_SMALL)
						{
							level--;
							unableReadyFly();
							StartUntouchable();
						}
						else
							SetState(MARIO_STATE_DIE);
					}
				}
				else if (dynamic_cast<ParaGoomba *>(e->obj)) // if e->obj is ParaGoomba 
				{
					ParaGoomba *paraGoomba = dynamic_cast<ParaGoomba *>(e->obj);

					// jump on top >> kill Goomba and deflect a bit 
					if (e->ny < 0)
					{

						if (paraGoomba->GetState() == PARA_GOOMBA_STATE_WALKING_WING)
						{
							paraGoomba->SetState(PARA_GOOMBA_STATE_WALKING);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else if (paraGoomba->GetState() == PARA_GOOMBA_STATE_WALKING)
						{
							paraGoomba->SetState(PARA_GOOMBA_STATE_DIE);
							paraGoomba->setTimeDie(GetTickCount());
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (paraGoomba->GetState() != PARA_GOOMBA_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									level--;
									unableReadyFly();
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				else if (dynamic_cast<ParaKoopa *>(e->obj)) // if e->obj is ParaKoopa 
				{
					ParaKoopa *paraKoopa = dynamic_cast<ParaKoopa *>(e->obj);

					if (paraKoopa->state != PARA_KOOPA_STATE_DIE_DISAPPER)
					{
						// jump on top >> kill Koopa
						if (e->ny < 0)
						{
							if (paraKoopa->GetState() == PARA_KOOPA_STATE_WALKING_WING)
							{
								paraKoopa->SetState(PARA_KOOPA_STATE_WALKING);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
							else if (paraKoopa->GetState() == PARA_KOOPA_STATE_WALKING)
							{
								paraKoopa->SetState(PARA_KOOPA_STATE_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
						}
						else if (e->nx > 0) // collision right to left
						{
							// Koopa die
							if (paraKoopa->GetState() == PARA_KOOPA_STATE_DIE)
							{
								// Hold koopa
								if (this->hold == hold)
								{
									paraKoopa->SetState(PARA_KOOPA_STATE_HOLDING);
									this->koopaHold = paraKoopa;
								}
								else
								{
									// Hit Koopa
									//this->SetState(MARIO_STATE_HIT);
									paraKoopa->SetState(PARA_KOOPA_STATE_THROWING_LEFT);
								}
							}
							else
							{
								// koopa live
								if (untouchable == 0)
								{
									if (level > MARIO_LEVEL_SMALL)
									{
										level--;
										unableReadyFly();
										StartUntouchable();
									}
									else
										SetState(MARIO_STATE_DIE);
								}
							}
						}
						else if (e->nx < 0) // collision left->right
						{
							if (paraKoopa->GetState() == PARA_KOOPA_STATE_DIE)
							{
								// Hold koopa
								if (this->hold == true)
								{
									paraKoopa->SetState(PARA_KOOPA_STATE_HOLDING);
									this->koopaHold = paraKoopa;
								}
								else
								{
									//this->SetState(MARIO_STATE_HIT);
									paraKoopa->SetState(PARA_KOOPA_STATE_THROWING_RIGHT);
								}

							}
							else
							{
								if (untouchable == 0)
								{
									if (level > MARIO_LEVEL_SMALL)
									{
										level--;
										unableReadyFly();
										StartUntouchable();
									}
									else
										SetState(MARIO_STATE_DIE);
								}
							}
						}
					}
				}
				else if (dynamic_cast<VenusFire *>(e->obj)) // if e->obj is VenusFire 
				{
					VenusFire *venusFire = dynamic_cast<VenusFire *>(e->obj);

					if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level--;
								unableReadyFly();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
				else if (dynamic_cast<Pihanra *>(e->obj)) // if e->obj is Pihanra 
				{
					Pihanra *pihanra = dynamic_cast<Pihanra *>(e->obj);

					if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level--;
								unableReadyFly();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}


		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mario::Render()
{
	if (level == MARIO_LEVEL_SWITCH_MAP) {
		animation_set->at(0)->Render(x, y);
	}
	else {
		int ani = -1;
		if (state == MARIO_STATE_DIE)
		{
			ani = MARIO_ANI_DIE;
		}
		else {
			if (level == MARIO_LEVEL_BIG)
			{
				if (hold == true && koopaHold != NULL)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_BIG_HOLD_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_HOLD_LEFT;
					}
				}
				else if (hit == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_HIT_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_HIT_LEFT;
					}
				}
				else
				{
					if (vx == 0)
					{
						if (ny > 0) // sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_BIG_SITTING_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_BIG_SITTING_LEFT;
							}
						}
						else // not sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_BIG_IDLE_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_BIG_IDLE_LEFT;
							}
						}

					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_BIG_WALKING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_WALKING_LEFT;
					}
				}

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (hold == true && koopaHold != NULL)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_SMALL_HOLD_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_HOLD_LEFT;
					}
				}
				else if (hit == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_HIT_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_HIT_LEFT;
					}
				}
				else
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SMALL_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SMALL_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_WALKING_LEFT;
					}
				}

			}
			else if (level == MARIO_LEVEL_SUPER_BIG)
			{
				if (readyFly == true && fly == false) { // Ready for state
					if (nx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_READY_FLY_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_READY_FLY_LEFT;
					}
				}
				else if (hold == true && koopaHold != NULL)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SUPER_BIG_IDLE_HOLD_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SUPER_BIG_IDLE_HOLD_LEFT;
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_HOLD_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_HOLD_LEFT;
					}
				}
				else if (hit == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_HIT_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_HIT_LEFT;
					}
				}
				else if (fly == true) {
					if (nx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_FLY_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_FLY_LEFT;
					}
				}
				else if (fall == true) {

					if (restrain == true) {
						if (nx > 0)
						{
							ani = MARIO_ANI_SUPER_BIG_RESTRAIN_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SUPER_BIG_RESTRAIN_LEFT;
						}
					}
					else
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SUPER_BIG_FALL_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SUPER_BIG_FALL_LEFT;
						}
					}
				}
				else if (attack == true) {
					if (nx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_ATTACK_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_ATTACK_LEFT;
					}
				}
				else {
					if (vx == 0)
					{
						if (ny > 0) // sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_SUPER_BIG_SITTING_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_SUPER_BIG_SITTING_LEFT;
							}
						}
						else // not sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_SUPER_BIG_IDLE_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_SUPER_BIG_IDLE_LEFT;
							}
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_WALKING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_WALKING_LEFT;
					}
				}
			}
			else if (level == MARIO_LEVEL_FIRE) {
				if (hold == true && koopaHold != NULL)
				{
					if (vx == 0)
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_FIRE_HOLD_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_FIRE_HOLD_IDLE_LEFT;
						}
					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_FIRE_HOLD_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_HOLD_LEFT;
					}
				}
				else if (hit == true)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_HIT_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_HIT_LEFT;
					}
				}
				else if (shot == true) {
					if (nx > 0)
					{
						ani = MARIO_ANI_FIRE_SHOT_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_SHOT_LEFT;
					}
				}
				else
				{
					if (vx == 0)
					{
						if (ny > 0) // sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_FIRE_SITTING_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_FIRE_SITTING_LEFT;
							}
						}
						else // not sit
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_FIRE_IDLE_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_FIRE_IDLE_LEFT;
							}
						}

					}
					else if (vx > 0)
					{
						ani = MARIO_ANI_FIRE_WALKING_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_FIRE_WALKING_LEFT;
					}
				}
			}
		}

		int alpha = 255;
		if (untouchable) alpha = 128;

		animation_set->at(ani)->Render(x, y, alpha);
	}
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (fast == true) vx = MARIO_WALKING_SPEED_FAST;
		else vx = MARIO_WALKING_SPEED;
		nx = 1;
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;// Not sit
		hit = false; // cancel state hit
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (fast == true) vx = -MARIO_WALKING_SPEED_FAST;
		else vx = -MARIO_WALKING_SPEED;
		nx = -1;
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;
		hit = false; // cancel state hit
		break;
	case MARIO_STATE_WALKING_DOWN:
		vy = MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_WALKING_UP:
		vy = -MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_JUMP:
		if (ny == -1) {
			DWORD t = GetTickCount() - lastJumpTime;
			// 0.5s sleep when mario jump
			if (t > 500 && isCollision == true)
			{
				lastJumpTime = GetTickCount();
				isCollision = false;
				vy = -MARIO_JUMP_SPEED_Y;
				ny = -1;
			}
		}
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		if (level == MARIO_LEVEL_SWITCH_MAP) { vy = 0; }
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;
		break;
	case MARIO_STATE_SITTING_RIGHT:
		vx = 0;
		ny = 1;
		break;
	case MARIO_STATE_SITTING_LEFT:
		vx = 0;
		ny = 1;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_HIT:
		hold = false;
		hit = true;
		vx = 0;
		ny = -1;
		break;
	}
}
void Mario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	// Calculate width - height mario
	float width = 0, height = 0;
	if (level == MARIO_LEVEL_BIG)
	{
		if (ny > 0 && hit == false && hold == false)
		{
			height = MARIO_BIG_BBOX_HEIGHT_SITTING;
		}
		else
		{
			height = MARIO_BIG_BBOX_HEIGHT;
		}
		if (hold == true && koopaHold != NULL)
		{
			width = MARIO_BIG_BBOX_WIDTH_HOLDING;
		}
		else {
			width = MARIO_BIG_BBOX_WIDTH;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (hold == true && koopaHold != NULL)
		{
			width = MARIO_SMALL_BBOX_WIDTH_HOLDING;
		}
		else {
			width = MARIO_SMALL_BBOX_WIDTH;
		}
		height = MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SUPER_BIG)
	{
		if (ny > 0 && hit == false && hold == false)
		{
			height = MARIO_SUPER_BIG_BBOX_HEIGHT_SITTING;
		}
		else
		{
			height = MARIO_SUPER_BIG_BBOX_HEIGHT;
		}
		if (hold == true && koopaHold != NULL)
		{
			width = MARIO_SUPER_BIG_BBOX_WIDTH_HOLDING;
		}
		else {
			width = MARIO_SUPER_BIG_BBOX_WIDTH;
		}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (ny > 0 && hit == false && hold == false)
		{
			height = MARIO_FIRE_BBOX_HEIGHT_SITTING;
		}
		else
		{
			height = MARIO_FIRE_BBOX_HEIGHT;
		}
		if (hold == true && koopaHold != NULL)
		{
			width = MARIO_FIRE_BBOX_WIDTH_HOLDING;
		}
		else {
			width = MARIO_FIRE_BBOX_WIDTH;
		}
	}
	else if (level == MARIO_LEVEL_SWITCH_MAP)
	{
		height = MARIO_SWITCH_MAP_HEIGHT;
		width = MARIO_SWITCH_MAP_WIDTH;
	}

	top = y;
	bottom = y + height;
	if (hold == true && koopaHold != NULL && nx < 0)
	{
		left = x - KOOPA_BBOX_WIDTH + 3;
	}
	else {
		left = x;
	}
	right = left + width;
}
void Mario::unableReadyFly()
{
	power = 0;
	for (size_t i = 0; i < hudSpeedList.size(); i++)
	{
		hudSpeedList[i]->disable = true;
	}
}
/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SUPER_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	unableReadyFly();
}

void Mario::UpLevel()
{
	if (level != MARIO_LEVEL_SUPER_BIG) {
		int l = ++level;
		SetPosition(x, y - MARIO_SUPER_BIG_BBOX_HEIGHT);
		SetLevel(l);
	}
}

void Mario::DownLevel()
{
	if (level != MARIO_LEVEL_SMALL) {
		int l = --level;
		SetPosition(x, y - MARIO_SUPER_BIG_BBOX_HEIGHT);
		SetLevel(l);
	}
}



