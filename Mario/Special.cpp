#include "Special.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Portal.h"
#define SCENE_MAP_SWITCH	2


void Special::Render()
{
	int ani = SPECIAL_ANI_NORMAL;

	if (state == SPECIAL_STATE_REWARD) ani = SPECIAL_ANI_REWARD;

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
		if (mario->x > END_MAP_1_1_POSITION_OUT_X) {
			mario->reward = NULL;
			Portal *p = new Portal(SCENE_MAP_SWITCH);
			Game::GetInstance()->SwitchScene(p->GetSceneId());
		}
	}
}