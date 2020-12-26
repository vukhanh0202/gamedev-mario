#include "Button.h"



void Button::Render()
{
	int ani = BUTTON_ANI_NORMAL;

	if (state == BUTTON_STATE_PRESS) ani = BUTTON_ANI_PRESS;

	animation_set->at(ani)->Render(x, y);
}

void Button::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	
	l = x;
	t = y;
	r = x + BUTTON_BBOX_WIDTH;
	if (state == BUTTON_STATE_PRESS) {
		b = y + BUTTON_BBOX_HEIGHT_PRESS;
	}
	else {
		b = y + BUTTON_BBOX_HEIGHT;
	}
}

void Button::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (position_default_y - 16 < y) {
		dy = dt * vy;
		y += dy;
	}
}
