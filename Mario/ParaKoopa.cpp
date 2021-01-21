#include "ParaKoopa.h"
#include "Ground.h"
#include "Brick.h"
#include "Box.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "Goomba.h"
#include "ParaGoomba.h"
#include "BrickGlass.h"
#include "BrokenBrick.h"



void ParaKoopa::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == PARA_KOOPA_STATE_HOLDING || state == PARA_KOOPA_STATE_DIE_DISAPPER) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + PARA_KOOPA_BBOX_WIDTH;

		if (state == PARA_KOOPA_STATE_WALKING_WING)
			bottom = y + PARA_KOOPA_BBOX_HEIGHT_WING;
		else if (state == PARA_KOOPA_STATE_WALKING)
			bottom = y + PARA_KOOPA_BBOX_HEIGHT;
		else
			bottom = y + PARA_KOOPA_BBOX_HEIGHT_DIE;
	}
}

void ParaKoopa::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	if (GetTickCount64() - timeJump >= PARA_KOOPA_TIME_JUMP && state == PARA_KOOPA_STATE_WALKING_WING ) {
		vy = -PARA_KOOPA_JUMP;
		timeJump = GetTickCount64();
	}
	// Calculate dx, dy 
	GameObject::Update(dt);

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();
	if (this->state == PARA_KOOPA_STATE_DIE && GetTickCount64() - timeDie > TIME_REVIVE) {
		y -= 10;
		SetState(PARA_KOOPA_STATE_WALKING);
	}

	if (state != PARA_KOOPA_STATE_DIE_DISAPPER)
		CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		if (state != PARA_KOOPA_STATE_HOLDING)
		{
			vy += PARA_KOOPA_GRAVITY * dt;
		}
		x += dx;
		y += dy;
	}
	else
	{
		
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		//// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		/*if (state == PARA_KOOPA_STATE_WALKING || state == PARA_KOOPA_STATE_WALKING_WING)
		{
			y += min_ty * dy + ny * 0.4f;
		}*/

		if (GetState() == PARA_KOOPA_STATE_HOLDING)
		{
			if (nx != 0) vx = 0;
		}
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (dynamic_cast<Boxs *>(e->obj)) // if e->obj is Koopa 
			{
				if (state == PARA_KOOPA_STATE_WALKING)
				{
					Boxs *boxs = dynamic_cast<Boxs *>(e->obj);
					double xBox, yBox;
					boxs->GetPosition(xBox, yBox);

					if (x >= boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5 || x <= xBox + BOX_BBOX_WIDTH / 2)
					{
						vx = -vx;
					}
					if (x > boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5)
					{
						x = boxs->getWidth() + xBox - BOX_BBOX_WIDTH * 1.5;
					}
					else if (x < xBox + BOX_BBOX_WIDTH / 2) {
						x = xBox + BOX_BBOX_WIDTH / 2;
					}
				}
				else if (state == PARA_KOOPA_STATE_WALKING_WING)
				{
					if (e->ny > 0)
					{
						y += dy;
					}
					else if (e->nx != 0)
					{
						x += dx;
					}
					else {
						if (ny != 0) vy = 0;
					}
				}

			}
			else if (dynamic_cast<Koopa *>(e->obj) && (this->state == PARA_KOOPA_STATE_THROWING_LEFT || this->state == PARA_KOOPA_STATE_THROWING_RIGHT)) {
				Koopa *koopa = dynamic_cast<Koopa *>(e->obj);
				if (koopa->state == PARA_KOOPA_STATE_WALKING) {
					if (this->nx > 0) {
						koopa->nx = 1;
					}
					else {
						koopa->nx = -1;
					}
					koopa->state = PARA_KOOPA_STATE_DIE_DISAPPER;
				}
				/*else {
					vx = -vx;
				}*/
			}
			else if (dynamic_cast<ParaKoopa *>(e->obj) && (this->state == PARA_KOOPA_STATE_THROWING_LEFT || this->state == PARA_KOOPA_STATE_THROWING_RIGHT)) {
				ParaKoopa *paraKoopa = dynamic_cast<ParaKoopa *>(e->obj);
				if (paraKoopa->state == PARA_KOOPA_STATE_WALKING || paraKoopa->state == PARA_KOOPA_STATE_WALKING_WING) {
					if (this->nx > 0) {
						paraKoopa->nx = 1;
					}
					else {
						paraKoopa->nx = -1;
					}
					paraKoopa->state = PARA_KOOPA_STATE_DIE_DISAPPER;
				}
				/*else {
					vx = -vx;
				}*/
			}
			else if (dynamic_cast<Goomba *>(e->obj) && (this->state == PARA_KOOPA_STATE_THROWING_LEFT || this->state == PARA_KOOPA_STATE_THROWING_RIGHT)) {
				Goomba *goomba = dynamic_cast<Goomba *>(e->obj);
				if (goomba->state == GOOMBA_STATE_WALKING) {
					goomba->state = GOOMBA_STATE_DIE_DISAPPER;
				}
			}
			else if (dynamic_cast<ParaGoomba *>(e->obj) && (this->state == PARA_KOOPA_STATE_THROWING_LEFT || this->state == PARA_KOOPA_STATE_THROWING_RIGHT)) {
				ParaGoomba *paraGoomba = dynamic_cast<ParaGoomba *>(e->obj);
				if (paraGoomba->state != PARA_GOOMBA_STATE_DIE) {
					paraGoomba->state = PARA_GOOMBA_STATE_DIE_DISAPPER;
				}
			}
			else if (dynamic_cast<BrickGlass *>(e->obj) && (this->state == KOOPA_STATE_THROWING_LEFT || this->state == KOOPA_STATE_THROWING_RIGHT)) {
				if (nx != 0)
				{
					Game *game = Game::GetInstance();
					LPScene scene = Game::GetInstance()->GetCurrentScene();

					BrickGlass *brickGlass = dynamic_cast<BrickGlass *>(e->obj);
					double brickX, brickY;
					brickGlass->GetPosition(brickX, brickY);

					// Brick 1
					GameObject *brokenBrick0 = new BrokenBrick();
					brokenBrick0->SetPosition(brickX, brickY);
					brokenBrick0->SetSpeed(0.2f, -0.3f);
					LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(26001);
					brokenBrick0->SetAnimationSet(ani_set);
					((PlayScene*)scene)->pushObject(brokenBrick0);
					// Brick 2
					GameObject *brokenBrick1 = new BrokenBrick();
					brokenBrick1->SetPosition(brickX, brickY);
					brokenBrick1->SetSpeed(-0.2f, -0.15f);
					brokenBrick1->SetAnimationSet(ani_set);
					((PlayScene*)scene)->pushObject(brokenBrick1);
					// Brick 3
					GameObject *brokenBrick2 = new BrokenBrick();
					brokenBrick2->SetPosition(brickX, brickY);
					brokenBrick2->SetSpeed(-0.1f, -0.3f);
					brokenBrick2->SetAnimationSet(ani_set);
					((PlayScene*)scene)->pushObject(brokenBrick2);
					// Brick 4
					GameObject *brokenBrick3 = new BrokenBrick();
					brokenBrick3->SetPosition(brickX, brickY);
					brokenBrick3->SetSpeed(0.1f, -0.15f);
					brokenBrick3->SetAnimationSet(ani_set);
					((PlayScene*)scene)->pushObject(brokenBrick3);

					brickGlass->disable = true;
					vx = -vx;
				}
			}
			else if (dynamic_cast<BrickQuestion *>(e->obj) && (this->state == KOOPA_STATE_THROWING_LEFT || this->state == KOOPA_STATE_THROWING_RIGHT)) {
				if (nx != 0)
				{
					BrickQuestion *brickQuestion = dynamic_cast<BrickQuestion *>(e->obj);
					brickQuestion->SetState(BRICK_STATE_EMPTY);
					vx = -vx;
				}
			}
			else if (nx != 0)
			{
				vx = -vx;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ParaKoopa::Render()
{
	int ani = PARA_KOOPA_ANI_WALKING_LEFT;
	if (state == PARA_KOOPA_STATE_DIE || state == PARA_KOOPA_STATE_HOLDING) {
		ani = PARA_KOOPA_ANI_DIE;
	}
	else if (state == PARA_KOOPA_STATE_THROWING_RIGHT || state == PARA_KOOPA_STATE_THROWING_LEFT) {
		ani = PARA_KOOPA_ANI_THROWING;
	}
	else if (state == PARA_KOOPA_STATE_DIE_DISAPPER) {
		ani = PARA_KOOPA_ANI_DIE_DISAPPEAR;
	}
	else if (state == PARA_KOOPA_STATE_WALKING_WING) {
		if (vx > 0) ani = PARA_KOOPA_ANI_WALKING_RIGHT_WING;
		else if (vx <= 0) ani = PARA_KOOPA_ANI_WALKING_LEFT_WING;
	}
	else if (vx > 0) ani = PARA_KOOPA_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = PARA_KOOPA_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

}

void ParaKoopa::SetState(int state)
{
	boolean flag = true;
	if (this->state == PARA_KOOPA_STATE_THROWING_RIGHT || this->state == PARA_KOOPA_STATE_THROWING_LEFT || state == PARA_KOOPA_STATE_DIE)
	{
		flag = false;
	}
	GameObject::SetState(state);
	switch (state)
	{
	case PARA_KOOPA_STATE_DIE:
		timeDie = GetTickCount64();
		if (vx != 0 && flag)
			y += PARA_KOOPA_BBOX_HEIGHT - PARA_KOOPA_BBOX_HEIGHT_DIE;
		vx = 0;
		break;
	case PARA_KOOPA_STATE_THROWING_LEFT:
		vx = -PARA_KOOPA_THROWING_SPEED;
		nx = -1;
		break;
	case PARA_KOOPA_STATE_THROWING_RIGHT:
		vx = PARA_KOOPA_THROWING_SPEED;
		nx = 1;
		break;
	case PARA_KOOPA_STATE_WALKING: case PARA_KOOPA_STATE_WALKING_WING:
		vx = -PARA_KOOPA_WALKING_SPEED;
		vy = 0;
		break;
	case PARA_KOOPA_STATE_HOLDING:
		vx = 0;
		vy = 0;
		break;
	case PARA_KOOPA_STATE_DIE_DISAPPER:
		vy = -PARA_KOOPA_DIE_DEFLECT_SPEED;
		if (nx > 0) {
			vx = PARA_KOOPA_WALKING_SPEED;
		}
		else {
			vx = -PARA_KOOPA_WALKING_SPEED;
		}
		break;
	}
}
ParaKoopa::ParaKoopa()
{
	nx = -1;
	SetState(PARA_KOOPA_STATE_WALKING_WING);
	vy = PARA_KOOPA_GRAVITY;
	timeJump = 0;
}

void ParaKoopa::CalcPotentialCollisions(vector<LPGameObject>* coObjects, vector<LPCollisionEvent>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCollisionEvent e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<Mario*>(coObjects->at(i)))
		{
			Mario *mario = dynamic_cast<Mario *>(e->obj);
			if (mario->getUntouchable() != 0)
				continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);

}

void KoopaVertical::Update(DWORD dt, vector<LPGameObject> *coObjects)
{

	// Calculate dx, dy 
	GameObject::Update(dt);
	y += dy;
	Game *game = Game::GetInstance();
	if ((y < game->GetScreenHeight() / 4 || y >game->GetScreenHeight() * 0.65f)) {
		if (state != PARA_KOOPA_STATE_DIE_FALL) {
			vy = -vy;
		}
	}
}
void KoopaVertical::SetState(int state)
{
	GameObject::SetState(state);
	if (state == PARA_KOOPA_STATE_DIE_FALL) {
		vy = KOOPA_DIE_DEFLECT_SPEED / 3;
	}
}

