#include "FallDrain.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "Portal.h"

void FallDrain::Render()
{
	animation_set->at(0)->Render(x, y);
}

void FallDrain::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + DRAIN_BBOX_WIDTH;
	b = y + DRAIN_BBOX_HEIGHT;
}
void FallDrainMap11::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();

	// Calculate dx, dy 
	GameObject::Update(dt);

	if (this->marioIn && !isEmpty) {
		int heightMario = mario->GetHeightDrainFall();
		if (mario->y > this->positionStartY && mario->y < this->positionInY - heightMario - 1) {
			mario->SetPosition(this->positionInX, positionInY - heightMario);
		}
		if (mario->y < this->positionInY - heightMario) {
			mario->SetPosition(this->positionStartX, mario->y + SPEED_SWAP_SCENE * dt);
		}
		else {
			mario->SetPosition(this->positionInX, mario->y + SPEED_SWAP_SCENE * dt);
		}
		
		if (mario->y > this->positionInY) {
			mario->setFallDrain(false);
			this->marioIn = false;
			this->isEmpty = true;
		}
	}
	if (abs(mario->y - this->positionOutY) < DISTANCE_SWAP && abs(mario->x - this->positionOutX) < DISTANCE_SWAP 
		&& marioOut == false && !mario->GetFly()) {
		mario->setFallDrain(true);
		this->marioOut = true;
	}
	if (this->marioOut) {
		mario->setFallDrain(true);
		int heightMario = mario->GetHeightDrainFall();
		if (mario->y + heightMario < this->positionOutY && mario->y > positionEndY + 1) {
			mario->SetPosition(this->positionEndX, positionEndY);
		}
		mario->SetPosition(mario->x, mario->y - SPEED_SWAP_SCENE *dt);

		if (mario->y + heightMario < this->positionEndY) {
			mario->setFallDrain(false);
			this->marioOut = false;
			this->isEmpty = true;
			mario->setInTunnel(false);
		}
	}

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();	

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		//// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];
			if (dynamic_cast<Mario *>(e->obj))
			{
				Mario *mario = dynamic_cast<Mario *>(e->obj);
				if (e->ny > 0 && isEmpty == false)
				{
					mario->setFallDrain(true);
					this->marioIn = true;
					mario->setInTunnel(true);
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void EndMap11::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}
void EndMap11::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	Game *game = Game::GetInstance();
	LPScene scene = Game::GetInstance()->GetCurrentScene();
	Mario *mario = ((PlayScene*)scene)->GetPlayer();

	// Calculate dx, dy 
	GameObject::Update(dt);
	if (mario->x > positionStartX) {
		mario->setNoAction(true);
		this->marioIn = true;
		if (mario->state != MARIO_STATE_WALKING_RIGHT) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		if (!mario->GetFly() && !mario->GetFall()) {
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			mario->SetSpeed(0.1f, 0.0f);
		}
		else {
			mario->SetSpeed(0.0f, 0.2f);
		}
	}
	if (mario->x > positionEndX) {
		Portal *p = new Portal(SCENE_MAP_SWITCH);
		Game::GetInstance()->SwitchScene(p->GetSceneId());
	}
	
}



