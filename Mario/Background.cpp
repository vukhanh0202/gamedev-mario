#include "Background.h"
#include "Game.h"
#include "PlayScene.h"


// Background
void BackGround::Render()
{
	animation_set->at(0)->Render(x, y);
}

void BackGround::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}


// Background motion
void BackGroundMotion::Render()
{
	animation_set->at(0)->Render(x, y);
}

void EffectAttack::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (GetTickCount64() - this->timeAppear >= TIME_EXIST_EFFECT) {
		this->disable = true;
	}
}

void BackGroundMotion::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}
void BackGroundMotion::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	BackGroundMotion *bg = ((PlayScene*)scene)->GetBGMotion();
	BackGroundMotionTile* tile = (BackGroundMotionTile*)bg->arr[OBJECT_TYPE_BACKGROUND_MOTION_TILE];
	BackGroundMotionLogoBlack* logoBlack = (BackGroundMotionLogoBlack*)bg->arr[OBJECT_TYPE_BACKGROUND_MOTION_LOGO_BLACK];
	BackGroundMotionLogoColor* logoColor = (BackGroundMotionLogoColor*)bg->arr[OBJECT_TYPE_BACKGROUND_MOTION_LOGO_COLOR];
	BackGroundMotionLogoNumber* logoNumber = (BackGroundMotionLogoNumber*)bg->arr[OBJECT_TYPE_BACKGROUND_MOTION_LOGO_NUMBER];

	BackGroundMotionLogoArrow* logoArrow1 = (BackGroundMotionLogoArrow*)bg->arrArrow[0];
	BackGroundMotionLogoArrow* logoArrow2 = (BackGroundMotionLogoArrow*)bg->arrArrow[1];
	
	if (tile->disable == true) {
		logoBlack->Show();
		if (logoBlack->y == 0 && tile->disable == true) {
			logoColor->Show();
			logoNumber->Show();
			if (logoArrow1->disable && logoArrow2->disable) {
				logoArrow1->Show();
				logoArrow2->Hide();
			}
		}
	}
	else {
		tile->Show();
	}
}

void BackGroundMotionTile::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (vy < 0 && y < -187) {
		y = -187;
		vy = 0;
		Hide();
	}

	// Calculate dx, dy 
	GameObject::Update(dt);

	x += dx;
	y += dy;
}

void BackGroundMotionLogoBlack::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (vy > 0 && y > 0) {
		y = 0;
		vy = 0;
		Hide();
	}

	// Calculate dx, dy 
	GameObject::Update(dt);

	x += dx;
	y += dy;
}
void BackGroundMotionLogoColor::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);
}
void BackGroundMotionLogoNumber::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);
}
void BackGroundMotionLogoArrow::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);
}