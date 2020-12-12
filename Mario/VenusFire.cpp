#include "VenusFire.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"

void VenusFire::Render()
{
	int ani = VENUS_FIRE_ANI_LOW_LEFT;
	if (fire) {
		if (nx > 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_RIGHT_FIRE;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_RIGHT_FIRE;
			}
		}
		else if (nx < 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_LEFT_FIRE;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_LEFT_FIRE;
			}
		}
	}
	else {
		if (nx > 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_RIGHT;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_RIGHT;
			}
		}
		else if (nx < 0) {
			if (ny > 0) {
				ani = VENUS_FIRE_ANI_HIGH_LEFT;
			}
			else {
				ani = VENUS_FIRE_ANI_LOW_LEFT;
			}
		}
	}


	animation_set->at(ani)->Render(x, y);
}

void VenusFire::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + VENUS_FIRE_BBOX_WIDTH;
	b = y + VENUS_FIRE_BBOX_HEIGHT;

}
void VenusFire::Update(DWORD dt, vector<LPGameObject> *coObjects)
{

	if (this->y <= this->position_default_y - VENUS_FIRE_BBOX_HEIGHT && !fire) {
		lastFireTime = GetTickCount();
		fire = true;
	}
	else if (fire && GetTickCount() - lastFireTime > VENUS_FIRE_TIME_FIRE) {
		fire = false;
		vy = VENUS_FIRE_SPEED;
	}
	if (fire) {
		vy = 0;
	}

	if (this->y > this->position_default_y && vy != 0) {
		lastAppearTime = GetTickCount();
		DebugOut(L"1\n");
	}

	if (GetTickCount() - lastAppearTime > VENUS_FIRE_TIME_WAIT && lastAppearTime != 0
		&& this->y > this->position_default_y - VENUS_FIRE_BBOX_HEIGHT && vy == 0) {
		vy = -VENUS_FIRE_SPEED;
	}
	else if ((GetTickCount() - lastAppearTime < VENUS_FIRE_TIME_WAIT) && this->y > this->position_default_y - VENUS_FIRE_BBOX_HEIGHT) {
		vy = 0;
		DebugOut(L"3\n");
	}

	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();
	if (mario->x > this->position_default_x) {
		this->nx = 1;
	}
	if (mario->x < this->position_default_x) {
		this->nx = -1;
	}
	if (mario->y < game->GetScreenHeight() / 2) {
		this->ny = 1;
	}
	if (mario->y > game->GetScreenHeight() / 2) {
		this->ny = -1;
	}

	// Calculate dx, dy 
	GameObject::Update(dt);

	x += dx;
	y += dy;
}
void VenusFire::SetState(int state)
{
	GameObject::SetState(state);

}
