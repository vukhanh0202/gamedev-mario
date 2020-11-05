#include "Koopa.h"
#include "Ground.h"
#include "Brick.h"
#include "Box.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"



void Koopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == KOOPA_STATE_HOLDING) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + KOOPA_BBOX_WIDTH;

		if (state == KOOPA_STATE_WALKING)
			bottom = y + KOOPA_BBOX_HEIGHT;
		else
			bottom = y + KOOPA_BBOX_HEIGHT_DIE;
	}
}

void Koopa::Update(DWORD dt, vector<LPGameObject> *coObjects)
{

	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();
	
	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (state != KOOPA_STATE_HOLDING)
		{
			vy += KOOPA_GRAVITY;
		}
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (GetState() == KOOPA_STATE_HOLDING)
		{
			if (nx != 0) vx = 0;
		}
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (dynamic_cast<Boxs *>(e->obj)) // if e->obj is Koopa 
			{
				if (state == KOOPA_STATE_WALKING)
				{
					Boxs *boxs = dynamic_cast<Boxs *>(e->obj);
					float xBox, yBox;
					boxs->GetPosition(xBox, yBox);

					if (x >= boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5 || x <= xBox + BOX_BBOX_WIDTH / 2)
					{
						vx = -vx;
					}
					if (x > boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5)
					{
						x > boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5;
					}
					else if (x < xBox + BOX_BBOX_WIDTH / 2) {
						x = xBox + BOX_BBOX_WIDTH / 2;
					}
				}
				
			}
			if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Koopa::Render()
{
	int ani = KOOPA_ANI_WALKING_LEFT;
	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_HOLDING) {
		ani = KOOPA_ANI_DIE;
	}
	else if (state == KOOPA_STATE_THROWING_RIGHT || state == KOOPA_STATE_THROWING_LEFT) {
		ani = KOOPA_ANI_THROWING;
	}
	else if (vx > 0) ani = KOOPA_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPA_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

}

void Koopa::SetState(int state)
{
	boolean flag = true;
	if (this->state == KOOPA_STATE_THROWING_RIGHT || this->state == KOOPA_STATE_THROWING_LEFT || state == KOOPA_STATE_DIE)
	{
		flag = false;
	}
	GameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		if (vx != 0 && flag == true)
			y += KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE;
		vx = 0;
		break;
	case KOOPA_STATE_THROWING_LEFT:
		vx = -KOOPA_THROWING_SPEED;
		break;
	case KOOPA_STATE_THROWING_RIGHT:
		vx = KOOPA_THROWING_SPEED;
		break;
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_HOLDING:
		vx = 0;
		vy = 0;
		break;
	}
}
Koopa::Koopa()
{
	SetState(KOOPA_STATE_WALKING);
	vy = KOOPA_GRAVITY;
}