#include "Hud.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#define WIDTH_END_BEFORE_PINE_MAP_14		2032
#define WIDTH_START_AFTER_PINE_MAP_14		2048

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
	Map *map = ((PlayScene*)scene)->map;
	double camX, camY;
	Game::GetInstance()->GetCamPosition(camX, camY);
	y =  camY + position_default_y - HUD_BOXX_HEIGHT;
	int width = game->GetScreenWidth();

	if (map) {
		/*if (mario->x < (game->GetScreenWidth() / 2)) {
			x = position_default_x;
		}*/
		/*if (mario->x > map->GetMapWidth() - (game->GetScreenWidth() / 2)) {
			x = map->GetMapWidth() - width + position_default_x;
		}
		else *///{
			x = camX + position_default_x;
		//}
		if (camX == WIDTH_END_BEFORE_PINE_MAP_14 - width) {
			x = WIDTH_END_BEFORE_PINE_MAP_14 - width + position_default_x;
		}
		else if (camX == WIDTH_START_AFTER_PINE_MAP_14) {
			x = WIDTH_START_AFTER_PINE_MAP_14 + position_default_x;
		}
	}
	else {
		if (mario->level == MARIO_LEVEL_SWITCH_MAP) {
			x = position_default_x;
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
}
