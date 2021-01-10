#include "Hud.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"



void Hud::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Hud::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}
void Hud::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();
	double camX, camY;
	Game::GetInstance()->GetCamPosition(camX, camY);
	y =  camY + position_default_y - HUD_BOXX_HEIGHT;
	if (((PlayScene*)scene)->map) {
		if (mario->x < (game->GetScreenWidth() / 2)) {
			x = position_default_x;
		}
		else if (mario->x > ((PlayScene*)scene)->map->GetMapWidth() - (game->GetScreenWidth() / 2)) {
			int width = game->GetScreenWidth();
			x = ((PlayScene*)scene)->map->GetMapWidth() - width + position_default_x;
		}
		else {
			x = mario->x - (game->GetScreenWidth() / 2) + position_default_x;
		}
	}
	else {
		if (mario->x < (game->GetScreenWidth() / 2)) {
			x = position_default_x;
		}
		else if (mario->x > END_MAP_1_1_POSITION_OUT_X - (game->GetScreenWidth() / 2)) {
			int width = game->GetScreenWidth();
			x = END_MAP_1_1_POSITION_OUT_X - width + position_default_x;
		}
		else {
			x = mario->x - (game->GetScreenWidth() / 2) + position_default_x;
		}
	}
}
