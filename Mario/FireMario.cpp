#include "FireMario.h"
#include "Ground.h"
#include "Box.h"
#include "Koopa.h"
#include "Goomba.h"

void FireMario::Render()
{
	int ani = FIRE_MARIO_ANI_RIGHT;

	if (vx > 0) ani = FIRE_MARIO_ANI_RIGHT;
	else if (vx <= 0) ani = FIRE_MARIO_ANI_LEFT;

	animation_set->at(ani)->Render(x, y);
}

void FireMario::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_FIRE_MARIO_WIDTH;
	b = y + BRICK_FIRE_MARIO_HEIGHT;

}
void FireMario::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;


	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	x += dx;
	y += dy;
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		vy += FIRE_MARIO_GRAVITY;
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

			if (dynamic_cast<Ground *>(e->obj) || dynamic_cast<Boxs *>(e->obj)) // if e->obj is Koopa 
			{
				y -= dy / 2;
				vy = -2 * FIRE_MARIO_DEFLECT;
			}
			else if (dynamic_cast<Koopa *>(e->obj)) {
				Koopa *koopa = dynamic_cast<Koopa *>(e->obj);
				if (koopa->state == KOOPA_STATE_WALKING) {
					if (this->nx > 0) {
						koopa->nx = 1;
					}
					else {
						koopa->nx = -1;
					}
					koopa->state = KOOPA_STATE_DIE_DISAPPER;
				}
				this->disable = true;
			}
			else if (dynamic_cast<Goomba *>(e->obj)) {
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
				this->disable = true;
			}
			else// if (nx != 0)
			{
				disable = true;
				SetState(FIRE_MARIO_STATE_DISABLE);
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	//}

}
void FireMario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case FIRE_MARIO_STATE_LEFT:
		vx = -FIRE_MARIO_SPEED;
		vy = 0;
		nx = -1;
		break;
	case FIRE_MARIO_STATE_RIGHT:
		vx = FIRE_MARIO_SPEED;
		vy = 0;
		nx = 1;
		break;
	case FIRE_MARIO_STATE_DISABLE:
		vx = 0;
		vy = 0;
		nx = 0;
		break;
	}
}