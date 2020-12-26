#include "Coin.h"



void Coin::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Coin::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}
void Coin::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);

	if (state == COIN_STATE_EARN) {
		vy += COIN_GRAVITY * dt;
		// Calculate dx, dy 

		x += dx;
		y += dy;
		if (this->y > this->position_default_y) {
			this->disable = true;
		}
	}
}
