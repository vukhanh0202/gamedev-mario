#include "Koopa.h"
#include "Ground.h"
#include "Brick.h"
#include "Box.h"



void Koopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPA_BBOX_WIDTH;

	if (state == KOOPA_STATE_WALKING)
		bottom = y + KOOPA_BBOX_HEIGHT;
	else
		bottom = y + KOOPA_BBOX_HEIGHT_DIE;
}

void Koopa::Update(DWORD dt, vector<LPGameObject> *coObjects)
{

	// Calculate dx, dy 
	GameObject::Update(dt);
	//vy += KOOPA_GRAVITY;

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	/*if (vx <= 0 && x < 0) {
		vx = -vx;
		x = 0;
	}*/
	coEvents.clear();
	if (state != KOOPA_STATE_DIE)
	{
		if (state == KOOPA_STATE_THROWING)
		{
			x += dx;
		}
		CalcPotentialCollisions(coObjects, coEvents);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
		{
			if (state != KOOPA_STATE_THROWING)
				vy += KOOPA_GRAVITY;
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
			x += dx;
			//x += min_tx * dx + nx * 0.4f;
			// RƠi mượt
			//y += min_ty * dy + ny * 0.4f;

			//for (UINT i = 0; i < coEventsResult.size(); i++)
			//{
			//	LPCollisionEvent e = coEventsResult[i];
			//	if (ny != 0) {
			//		x += dx;
			//	}
			//	if (nx != 0)
			//	{
			//		vx = -vx;
			//	}

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCollisionEvent e = coEventsResult[i];
				if (dynamic_cast<Brick *>(e->obj)) // if e->obj is Koopa 
				{
					Brick *brick = dynamic_cast<Brick *>(e->obj);
					if (e->nx != 0)
					{
						vx = -vx;
						//x += dx;
					}
				}
				else if (dynamic_cast<Ground *>(e->obj)) // if e->obj is Koopa 
				{
					Ground *ground = dynamic_cast<Ground *>(e->obj);
					if (e->nx != 0)
					{
						vx = -vx;
						//x += dx;
					}
				}

			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}


}

void Koopa::Render()
{
	int ani = KOOPA_ANI_WALKING_LEFT;
	if (state == KOOPA_STATE_DIE) {
		ani = KOOPA_ANI_DIE;
	}
	else if (state == KOOPA_STATE_THROWING) {
		ani = KOOPA_ANI_THROWING;
	}
	else if (vx > 0) ani = KOOPA_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPA_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);
}

void Koopa::SetState(int state)
{
	boolean flag = true;
	if (this->state == KOOPA_STATE_THROWING && state == KOOPA_STATE_DIE)
	{
		flag = false;
	}
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		if (vx != 0 && flag == true)
			y += KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_THROWING:
		//y += KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE + 1;
		//vx = KOOPA_THROWING_SPEED;
		vx = KOOPA_THROWING_SPEED;
		vy = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
	}
}
Koopa::Koopa()
{
	SetState(KOOPA_STATE_WALKING);
	vy = KOOPA_GRAVITY;
}