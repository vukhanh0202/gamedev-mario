#include "Goomba.h"


void Goomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE_DISAPPER) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}

}

void Goomba::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	vy += GOOMBA_GRAVITY * dt;

	// Calculate dx, dy 
	GameObject::Update(dt, coObjects);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();
	if (state == GOOMBA_STATE_DIE) {
		if (GetTickCount() - timeDie >= GOOMBA_TIME_DISAPPEAR) {
			disable = true;
		}
	}

	if (state != GOOMBA_STATE_DIE_DISAPPER)
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
		if (state != GOOMBA_STATE_DIE)
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

void Goomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_DISAPPER) {
		ani = GOOMBA_ANI_DIE_DISAPPER;
	}

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Goomba::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_DISAPPER:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		if (nx > 0) {
			vx = GOOMBA_WALKING_SPEED;
		}
		else {
			vx = -GOOMBA_WALKING_SPEED;
		}
		break;
	}
}
Goomba::Goomba()
{
	nx = -1;
	SetState(GOOMBA_STATE_WALKING);
	timeDie = 0;
}

