#include "FireEnemy.h"
#include "Box.h"
#include "Mario.h"
#include <algorithm>

void FireEnemy::Render()
{
	int ani = FIRE_ENEMY_ANI_RIGHT;

	if (vx > 0) ani = FIRE_ENEMY_ANI_RIGHT;
	else if (vx <= 0) ani = FIRE_ENEMY_ANI_LEFT;

	animation_set->at(ani)->Render(x, y);
}

void FireEnemy::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_FIRE_ENEMY_WIDTH;
	b = y + BRICK_FIRE_ENEMY_HEIGHT;

}
void FireEnemy::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (GetTickCount64() - timeAppear > TIME_FIRE_ENEMY_EXIST) {
		disable = true;
	}
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;


	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	x += dx;
	y += dy;
	//// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		//vy += FIRE_MARIO_GRAVITY;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		//// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (dynamic_cast<Boxs *>(e->obj)) // if e->obj is Box
			{
			}
			else// if (nx != 0)
			{
				disable = true;
				SetState(FIRE_ENEMY_STATE_DISABLE);
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//}

}
void FireEnemy::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case FIRE_ENEMY_STATE_LEFT:
		vx = -FIRE_ENEMY_SPEED;
		nx = -1;
		break;
	case FIRE_ENEMY_STATE_RIGHT:
		vx = FIRE_ENEMY_SPEED;
		nx = 1;
		break;
	case FIRE_ENEMY_STATE_DISABLE:
		vx = 0;
		vy = 0;
		nx = 0;
		break;
	}
}

void FireEnemy::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
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