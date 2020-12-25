#include "ParaGoomba.h"
#include "Mario.h"
#include <algorithm>

void ParaGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (state == PARA_GOOMBA_STATE_DIE_DISAPPER) {
		left = top = right = bottom = 0;
	}
	else if (state == PARA_GOOMBA_STATE_WALKING_WING) {
		right = x + PARA_GOOMBA_BBOX_WIDTH_SWING;
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT_SWING;
	}
	else if (state == PARA_GOOMBA_STATE_WALKING) {
		right = x + PARA_GOOMBA_BBOX_WIDTH;
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT;
	}
	else {
		right = x + PARA_GOOMBA_BBOX_WIDTH;
		bottom = y + PARA_GOOMBA_BBOX_HEIGHT_DIE;
	}

}

void ParaGoomba::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	vy += PARA_GOOMBA_GRAVITY * dt;

	if (GetTickCount64() - timeJump >= PARA_GOOMBA_TIME_JUMP && state == PARA_GOOMBA_STATE_WALKING_WING && timeJump != 0) {
		vy = -PARA_GOOMBA_JUMP;
		timeJump = GetTickCount64();
	}


	// Calculate dx, dy 
	GameObject::Update(dt, coObjects);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();
	if (state == PARA_GOOMBA_STATE_DIE) {
		if (GetTickCount64() - timeDie >= PARA_GOOMBA_TIME_DISAPPEAR) {
			disable = true;
		}
	}


	if (state != PARA_GOOMBA_STATE_DIE_DISAPPER)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		if (timeJump == 0) {
			timeJump = GetTickCount64();
		}
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		//// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		// block every object first!
		if (state != PARA_GOOMBA_STATE_DIE)
			y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.4f;

		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ParaGoomba::Render()
{
	int ani = PARA_GOOMBA_ANI_WALKING_WING;
	if (state == PARA_GOOMBA_STATE_WALKING) {
		ani = PARA_GOOMBA_ANI_WALKING;
	}
	else if (state == PARA_GOOMBA_STATE_DIE) {
		ani = PARA_GOOMBA_ANI_DIE;
	}
	else if (state == PARA_GOOMBA_STATE_DIE_DISAPPER) {
		ani = PARA_GOOMBA_ANI_DIE_DISAPPER;
	}

	animation_set->at(ani)->Render(x, y);
}

void ParaGoomba::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case PARA_GOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case PARA_GOOMBA_STATE_WALKING_WING: case PARA_GOOMBA_STATE_WALKING:
		vx = -PARA_GOOMBA_WALKING_SPEED;
		//vy = 0;
		break;
	case PARA_GOOMBA_STATE_DIE_DISAPPER:
		vy = -PARA_GOOMBA_WALKING_SPEED;
			vx = PARA_GOOMBA_WALKING_SPEED;
		if (nx > 0) {
		}
		else {
			vx = -PARA_GOOMBA_WALKING_SPEED;
		}
		break;
	}
}
void ParaGoomba::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
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

