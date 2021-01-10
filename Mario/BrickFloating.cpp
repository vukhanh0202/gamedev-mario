#include "BrickFloating.h"

BrickFloating::BrickFloating()
{
	state = BRICK_FLOATING_STATE_FLOAT;
}

void BrickFloating::Render()
{
	animation_set->at(0)->Render(x, y);
}

void BrickFloating::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_FLOATING_BBOX_WIDTH;
	b = y + BRICK_FLOATING_BBOX_HEIGHT;

}
void BrickFloating::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (state == BRICK_FLOATING_STATE_FALL) {
		vy += BRICK_FLOATING_FALL *dt;
		vx = 0;
	}
	else {
		vx = -BRICK_FLOATING_WALKING_SPEED;
		vy = 0;
	}
	// Calculate dx, dy 
	GameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}


