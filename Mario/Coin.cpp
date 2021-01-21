#include "Coin.h"
#include "Koopa.h"
#include "Game.h"
#include "Brick.h"


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

			// block every object first!
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCollisionEvent e = coEventsResult[i];

				if (dynamic_cast<Koopa *>(e->obj)) // if e->obj is Koopa 
				{
					Koopa *koopa = dynamic_cast<Koopa *>(e->obj);
					if (e->ny > 0) {
						if (koopa->state != KOOPA_STATE_DIE) {
							koopa->SetState(KOOPA_STATE_DIE);
						}
					}

				}

			}
		}
		if (this->y > this->position_default_y) {
			this->disable = true;
		}
	}
	else if (state == COIN_STATE_TIMEOUT) {
		if (GetTickCount64() - timeAppear >= TIME_COIN_TIME_OUT) {
			Game *game = Game::GetInstance();
			LPScene scene = Game::GetInstance()->GetCurrentScene();

			GameObject *glass = new BrickGlass();
			glass->SetPosition(x, y);

			LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(BRICK_GLASS_ANI_SET_ID);

			glass->SetAnimationSet(ani_set);

			((PlayScene*)scene)->pushObject(glass);
			this->disable = true;
		}
	}
}
