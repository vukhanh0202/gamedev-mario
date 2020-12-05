#include "Portal.h"


Portal::Portal(float l, float t, float r, float b, int scene_id)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}
Portal::Portal(int scene_id)
{
	this->scene_id = scene_id;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

void Portal::Render()
{
	RenderBoundingBox();
}

void Portal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}