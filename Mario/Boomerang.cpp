#include "Boomerang.h"
#include "Mario.h"


void Boomerang::Render()
{
	int ani = BOOMERANG_ANI_LEFT;

	if (vx > 0) ani = BOOMERANG_ANI_RIGHT;
	else if (vx <= 0) ani = BOOMERANG_ANI_LEFT;

	animation_set->at(ani)->Render(x, y);
}
void Boomerang::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
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
void Boomerang::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}
void Boomerang::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case BOOMERANG_STATE_LEFT:
		vx = -BOOMERANG_SPEED;
		nx = -1;
		break;
	case BOOMERANG_STATE_RIGHT:
		vx = BOOMERANG_SPEED;
		nx = 1;
		break;
	case BOOMERANG_STATE_DISABLE:
		vx = 0;
		vy = 0;
		nx = 0;
		break;
	}
}
void Boomerang::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Calculate dx, dy 
	if (vy <= -MAX_SPEED_Y) {
		vy = 0;
		back = 0;
	}
	if (back == 1) {
		vy -= BOOMERANG_SPEED_DEFLECT;
	}
	if (back == 0 && this->x < (start_x - 5)*direct) {
		this->disable = true;
	}
	GameObject::Update(dt);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;


	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	x += dx;
	y += dy;
	if (coEvents.size() != 0)
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

		}
	}
	if (this->x >= start_x + MAX_DISTANCE_X) {
		vx = -vx;
		back = 1;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}