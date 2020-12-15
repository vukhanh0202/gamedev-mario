#include "Ground.h"

void Ground::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Ground::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + GROUND_BBOX_WIDTH;
	b = y + GROUND_BBOX_HEIGHT;

}


