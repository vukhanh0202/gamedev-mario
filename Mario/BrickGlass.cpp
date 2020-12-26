#include "BrickGlass.h"
#include "Koopa.h"


void BrickGlass::Render()
{
	int ani = BRICK_ANI_NORMAL;

	if (state == BRICK_STATE_BROKEN) ani = BRICK_ANI_BROKEN;

	animation_set->at(ani)->Render(x, y);
}

void BrickGlass::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state == BRICK_STATE_BROKEN) {
		l = t = r = b = 0;
	}
	else {
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}
