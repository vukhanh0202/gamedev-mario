#include "VenusFire.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "FireEnemy.h"

void VenusFire::Render()
{
	int ani = VENUS_FIRE_ANI_LOW_LEFT;
	if (fire) {
		if (nx > 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_RIGHT_FIRE;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_RIGHT_FIRE;
			}
		}
		else if (nx < 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_LEFT_FIRE;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_LEFT_FIRE;
			}
		}
	}
	else {
		if (nx > 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_RIGHT;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_RIGHT;
			}
		}
		else if (nx < 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_LEFT;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_LEFT;
			}
		}
	}


	animation_set->at(ani)->Render(x, y);
}

void VenusFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + VENUS_FIRE_BBOX_WIDTH;
	b = y + VENUS_FIRE_BBOX_HEIGHT;

}
void VenusFire::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();

	if (this->y <= this->position_default_y - VENUS_FIRE_BBOX_HEIGHT && !fire) {
		lastFireTime = GetTickCount64();
		fire = true;
		GameObject *fireEnemy = new FireEnemy();
		if (this->nx > 0)
		{
			fireEnemy->SetState(FIRE_ENEMY_STATE_RIGHT);
			fireEnemy->SetPosition(this->x , this->y);
		}
		else {
			fireEnemy->SetState(FIRE_ENEMY_STATE_LEFT);
			fireEnemy->SetPosition(this->x, this->y);
		}
		double fireEnemyVx, fireEnemyVy;
		double positionFireEnemyX, positionFireEnemyY;
		double marioX, marioY;
		double distanceX, distanceY;
		// Get speed fire enemy
		fireEnemy->GetSpeed(fireEnemyVx, fireEnemyVy);
		// Get position fire enemy
		fireEnemy->GetPosition(positionFireEnemyX, positionFireEnemyY);
		// Get position mario
		mario->GetPosition(marioX, marioY);
		// cal distance
		distanceX = marioX - positionFireEnemyX;
		distanceY = marioY - positionFireEnemyY;

		fireEnemyVy = (distanceY / distanceX) *fireEnemyVx;
		fireEnemy->SetSpeed(fireEnemyVx, fireEnemyVy);
		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(FIRE_MARIO_ANIMATION_SET_ID);

		fireEnemy->SetAnimationSet(ani_set);
		((PlayScene*)scene)->pushObject(fireEnemy);
	}
	else if (fire && GetTickCount64() - lastFireTime > VENUS_FIRE_TIME_FIRE) {
		fire = false;
		vy = VENUS_FIRE_SPEED;
	}
	if (fire) {
		vy = 0;
	}

	if (this->y > this->position_default_y && vy != 0) {
		lastAppearTime = GetTickCount64();
	}

	if (GetTickCount64() - lastAppearTime > VENUS_FIRE_TIME_WAIT && lastAppearTime != 0
		&& this->y > this->position_default_y - VENUS_FIRE_BBOX_HEIGHT && vy == 0) {
		vy = -VENUS_FIRE_SPEED;
	}
	else if ((GetTickCount64() - lastAppearTime < VENUS_FIRE_TIME_WAIT) && this->y > this->position_default_y - VENUS_FIRE_BBOX_HEIGHT) {
		vy = 0;
	}

	
	if (mario->x > this->position_default_x) {
		this->nx = 1;
	}
	if (mario->x < this->position_default_x) {
		this->nx = -1;
	}
	if (mario->y < game->GetScreenHeight() / 2) {
		this->ny = 1;
	}
	if (mario->y > game->GetScreenHeight() / 2) {
		this->ny = -1;
	}

	// Calculate dx, dy 
	GameObject::Update(dt);
	x += dx;
	y += dy;
}
void VenusFire::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCollisionEvent e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<Mario*>(coObjects->at(i)))
		{
			Mario *mario = dynamic_cast<Mario *>(e->obj);
			if (mario->getUntouchable() != 0)
				continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);

}


