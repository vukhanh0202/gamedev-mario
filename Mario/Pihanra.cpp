#include "Pihanra.h"



void Pihanra::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Pihanra::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + PIHANRA_BBOX_WIDTH;
	b = y + PIHANRA_BBOX_HEIGHT;

}
void Pihanra::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Down
	if (this->y < this->position_default_y - PIHANRA_BBOX_HEIGHT && vy != 0) {
		lastFireTime = GetTickCount();
	}

	if (GetTickCount() - lastFireTime > PIHANRA_TIME_FIRE && lastFireTime != 0
		&& this->y < this->position_default_y - PIHANRA_BBOX_HEIGHT && vy == 0) {
		vy = PIHANRA_SPEED;
	}
	else if ((GetTickCount() - lastFireTime < PIHANRA_TIME_FIRE) && this->y < this->position_default_y - PIHANRA_BBOX_HEIGHT) {
		vy = 0;
	}

	// Up
	if (this->y > this->position_default_y && vy != 0) {
		lastAppearTime = GetTickCount();
	}

	if (GetTickCount() - lastAppearTime > PIHANRA_TIME_WAIT && lastAppearTime != 0
		&& this->y > this->position_default_y - PIHANRA_BBOX_HEIGHT && vy == 0) {
		vy = -PIHANRA_SPEED;
	}
	else if ((GetTickCount() - lastAppearTime < PIHANRA_TIME_WAIT) && this->y > this->position_default_y - PIHANRA_BBOX_HEIGHT) {
		vy = 0;
	}
	// Calculate dx, dy 
	GameObject::Update(dt);

	x += dx;
	y += dy;
}

