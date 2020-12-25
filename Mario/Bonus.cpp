#include "Bonus.h"
#include "Brick.h"


void Bonus::Render()
{
	int ani = BONUS_ANI_MUSHROOM_RED;

	if (state == BONUS_STATE_LEAF) ani = BONUS_ANI_LEAF;

	animation_set->at(ani)->Render(x, y);
}

void Bonus::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BONUS_BBOX_WIDTH;
	b = y + BONUS_BBOX_HEIGHT;
}
void Bonus::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (state == BONUS_STATE_MUSHROOM_RED) {
		vx = nx > 0 ? BONUS_SPEED : -BONUS_SPEED;
		vy += BONUS_GRAVITY * dt;

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
			y += min_ty * dy + ny * 0.4f;
			x += min_tx * dx + nx * 0.4f;

			if (ny != 0) vy = 0;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCollisionEvent e = coEventsResult[i];

				if (nx != 0 && (dynamic_cast<Brick *>(e->obj)))
				{
					vx = -vx;
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	else if (state == BONUS_STATE_LEAF) {
		if (GetTickCount64() - lastReverseSpeed > 500) {
			vx = vx > 0 ? -BONUS_SPEED_LEAF : BONUS_SPEED_LEAF;
			lastReverseSpeed = GetTickCount64();
		}
		else {
			if (vx > 0) {
				vx += BONUS_SPEED_LEAF;
			}
			else {
				vx -= BONUS_SPEED_LEAF;
			}
		}
		vy += BONUS_GRAVITY_LEAF;
		// Calculate dx, dy 
		GameObject::Update(dt, coObjects);

		x += dx;
		y += dy;
	}
}

