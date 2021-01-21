#include "BrotherBoom.h"
#include "Mario.h"
#include <algorithm>
#include "Game.h"
#include "Boomerang.h"
#include "Brick.h"


void BrotherBoom::Render()
{
	int ani = BROTHER_BOOM_ANI_WALKING_LEFT;
	if (state == BROTHER_BOOM_STATE_DIE) {
		ani = BROTHER_BOOM_ANI_DIE;
	}
	else if (state == BROTHER_BOOM_STATE_IDLE) {
		if (nx > 0) ani = BROTHER_BOOM_ANI_IDLE_RIGHT;
		else if (nx <= 0) ani = BROTHER_BOOM_ANI_IDLE_LEFT;
	}
	else {
		if (vx > 0) ani = BROTHER_BOOM_ANI_WALKING_RIGHT;
		else if (vx <= 0) ani = BROTHER_BOOM_ANI_WALKING_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
}
void BrotherBoom::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
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
void BrotherBoom::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{

	case BROTHER_BOOM_STATE_WALKING:
		vx = -BROTHER_BOOM_WALKING_SPEED;
		vy = 0;
		break;
	case BROTHER_BOOM_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BROTHER_BOOM_STATE_DIE:
		vy = -BROTHER_BOOM_DIE_DEFLECT_SPEED;
		if (nx > 0) {
			vx = BROTHER_BOOM_WALKING_SPEED;
		}
		else {
			vx = -BROTHER_BOOM_WALKING_SPEED;
		}
		break;
	}
}
BrotherBoom::BrotherBoom()
{
	nx = 1;
	vy = 0;
	SetState(BROTHER_BOOM_STATE_WALKING);
	timeAttack = 0;
	delayFirstTime = 2000;
}
void BrotherBoom::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == BROTHER_BOOM_STATE_DIE) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + BROTHER_BOOM_BBOX_WIDTH;
		bottom = y + BROTHER_BOOM_BBOX_HEIGHT;
	}
}
void BrotherBoom::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (GetTickCount64() - timeIdle <= DEPLAY_IDLE && delayFirstTime == 0) {
		SetState(BROTHER_BOOM_STATE_IDLE);
	}
	else {
		if (state == BROTHER_BOOM_STATE_IDLE) {
			state = BROTHER_BOOM_STATE_WALKING;
			if (nx > 0) {
				vx = BROTHER_BOOM_WALKING_SPEED;
			}
			else {
				vx = -BROTHER_BOOM_WALKING_SPEED;
			}
		}
		if (GetTickCount64() - timeAttack >= DEPLAY_ATTACK) {
			if (this->delayFirstTime != 0) {
				delayFirstTime = 0;
				timeAttack = GetTickCount();
			}
			else {
				Game *game = Game::GetInstance();
				LPScene scene = Game::GetInstance()->GetCurrentScene();
				Mario *mario = ((PlayScene*)scene)->GetPlayer();
				if (abs(mario->x - this->x) <= game->GetScreenWidth() / 2 && abs(mario->y - this->y) <= game->GetScreenHeight()) {
					GameObject *bullet;
					if (this->nx > 0)
					{
						bullet = new Boomerang(this->x + MARIO_BIG_BBOX_WIDTH * 1.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
						bullet->SetState(BOOMERANG_STATE_RIGHT);
						bullet->SetPosition(this->x + MARIO_BIG_BBOX_WIDTH * 1.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
						((Boomerang*)bullet)->direct = 1;
					}
					else {
						bullet = new Boomerang(this->x - MARIO_BIG_BBOX_WIDTH * 0.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
						bullet->SetState(BOOMERANG_STATE_LEFT);
						bullet->SetPosition(this->x - MARIO_BIG_BBOX_WIDTH * 0.1f, this->y + MARIO_BIG_BBOX_HEIGHT * 0.2);
						((Boomerang*)bullet)->direct = -1;
					}
					double fireEnemyVx, fireEnemyVy;
					double positionFireEnemyX, positionFireEnemyY;
					// Get speed fire enemy
					bullet->GetSpeed(fireEnemyVx, fireEnemyVy);
					// Get position fire enemy
					bullet->SetSpeed(fireEnemyVx, 0);
					LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(BOOMERANG_ANIMATION_SET_ID);
					this->nx = (mario->x - this->x) / abs(this->x - mario->x);
					bullet->SetAnimationSet(ani_set);
					((PlayScene*)scene)->pushObject(bullet);
				}
				timeAttack = GetTickCount64();
				timeIdle = GetTickCount64();
			}
		}
		vy += BROTHER_BOOM_GRAVITY * dt;
		// Calculate dx, dy 
		GameObject::Update(dt, coObjects);

		vector<LPCollisionEvent> coEvents;
		vector<LPCollisionEvent> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;
			//// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			// block every object first!
			if (state != BROTHER_BOOM_STATE_DIE)
				y += min_ty * dy + ny * 0.4f;
			x += min_tx * dx + nx * 0.4f;

			if (ny != 0) vy = 0;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCollisionEvent e = coEventsResult[i];
				if (dynamic_cast<Brick *>(e->obj)) // if e->obj is fireEnemy
				{
					if (e->nx != 0)
					{
						vx = -vx;
					}
				}

			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}