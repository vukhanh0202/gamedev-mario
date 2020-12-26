#include "BrokenBrick.h"


void BrokenBrick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void BrokenBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}
void BrokenBrick::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	vy += 0.0015f * dt;
	GameObject::Update(dt);

	x += dx;
	y += dy;
}


