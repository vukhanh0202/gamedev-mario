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
	if (mario->x < (game->GetScreenWidth() / 2)) {
		x = position_default_x;
	}
	else {
		x = mario->x - (game->GetScreenWidth() / 2) + position_default_x;
	}

	if ((mario->y < game->GetScreenHeight() / 4 && mario->GetFly())
		|| (mario->y <= game->GetScreenHeight() / 4 && mario->GetFall())
		|| (mario->y <= -game->GetScreenHeight() / 4)) {
		if (y < 0) {
			y = position_default_y - game->GetScreenHeight()*1.055;
		}
		else {
			y = mario->y + (game->GetScreenHeight() / 2);
		}

	}
	if (mario->y > 0) {
		y = position_default_y;
	}


}
