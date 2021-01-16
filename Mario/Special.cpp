#include "Special.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Portal.h"
#define WIDTH_END_BEFORE_PINE_MAP_14		2032
#define WIDTH_START_AFTER_PINE_MAP_14		2048
#define SCENE_MAP_SWITCH	2


void Special::Render()
{
	int ani = SPECIAL_ANI_NORMAL;

	if (state == SPECIAL_STATE_REWARD) ani = SPECIAL_ANI_REWARD;
	else if (state == SPECIAL_STATE_REWARD_COMPLETED) ani = SPECIAL_ANI_REWARD_COMPLETED;

	animation_set->at(ani)->Render(x, y);
}

void Special::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + SPECIAL_BBOX_WIDTH;
	b = y + SPECIAL_BBOX_HEIGHT;
}
void Special::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (state == SPECIAL_STATE_REWARD) {
		if (this->y > 0) {
			vy = -0.15f;
		}
		GameObject::Update(dt);
		y += dy;
		if (this->y < 0) {
			vy = 0;
			//disable = true;
		}

	}
	if (state == SPECIAL_STATE_REWARD) {
		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Mario *mario = ((PlayScene*)scene)->GetPlayer();

		// Calculate dx, dy 
		GameObject::Update(dt);
		mario->setNoAction(true);
		if (mario->state != MARIO_STATE_WALKING_RIGHT) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		if (!mario->GetFly() && !mario->GetFall()) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			mario->SetSpeed(0.05f, 0.2f);
		}
		else {
			mario->SetSpeed(0.0f, 0.2f);
		}
		int id = scene->getId();
		switch (id)
		{
		case 3: // scene 1
			if (mario->x > END_MAP_1_1_POSITION_OUT_X && Game::GetInstance()->rewards.size() == 1) {
				Game::GetInstance()->rounds.push_back(1);
				mario->reward = NULL;
				Portal *p = new Portal(SCENE_MAP_SWITCH);
				Game::GetInstance()->permitLoad = true;
				Game::GetInstance()->SetValueStore(mario->GetLevel(), mario->getPoint(), mario->getScore());
				Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
			break;
		case 4: // scene 2
			if (mario->x > ((PlayScene*)scene)->map->GetMapWidth() && Game::GetInstance()->rewards.size() == 2) {
				Game::GetInstance()->rounds.push_back(4);
				mario->reward = NULL;
				Portal *p = new Portal(SCENE_MAP_SWITCH);
				Game::GetInstance()->permitLoad = true;
				Game::GetInstance()->SetValueStore(mario->GetLevel(), mario->getPoint(), mario->getScore());
				Game::GetInstance()->SwitchScene(p->GetSceneId());
			}
			break;
		default:
			break;
		}

	}

	if (state == SPECIAL_STATE_REWARD_COMPLETED) {
		// Calculate dx, dy 
		GameObject::Update(dt);

		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Map *map = ((PlayScene*)scene)->map;
		double camX, camY;
		Game::GetInstance()->GetCamPosition(camX, camY);
		int width = game->GetScreenWidth();
		if (map) {
			y = camY + position_default_y;
			x = camX + position_default_x;
			if (camX == WIDTH_END_BEFORE_PINE_MAP_14 - width) {
				x = WIDTH_END_BEFORE_PINE_MAP_14 - width + position_default_x;
			}
			else if (camX == WIDTH_START_AFTER_PINE_MAP_14) {
				x = WIDTH_START_AFTER_PINE_MAP_14 + position_default_x;
			}
		}
	}
}