#include "Brick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Bonus.h"
#include "Button.h"
#include "Koopa.h"

void Brick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

// Brick question
void BrickQuestion::Render()
{
	int ani = BRICK_ANI_NORMAL;

	if (state == BRICK_STATE_EMPTY) ani = BRICK_ANI_EMPTY;

	animation_set->at(ani)->Render(x, y);
}

void BrickQuestionCoin::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);

	if (this->y < positionDefaultY) {
		vy = BRICK_DEFLECT_SPEED;
	}
	else {
		vy = 0;
	}
	y += dy;
	if (y > positionDefaultY) y = positionDefaultY;

	if (this->isEmpty == false && state == BRICK_STATE_EMPTY) {

		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Mario *mario = ((PlayScene*)scene)->GetPlayer();

		GameObject *coin = new Coin(this->x + BRICK_BBOX_WIDTH / 2, this->y - COIN_BBOX_WIDTH);
		coin->SetPosition(this->x + BRICK_BBOX_WIDTH / 4, this->y - COIN_BBOX_WIDTH);
		coin->SetState(COIN_STATE_EARN);

		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(COIN_ANIMATION_SET_ID);

		coin->SetAnimationSet(ani_set);

		((PlayScene*)scene)->pushObject(coin);
		mario->setScore(mario->getScore() + SCORE_PLUS);
		mario->setPoint(mario->getPoint() + 1);
		isEmpty = true;
	}
}
void BrickQuestionBonus::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);

	if (this->y < positionDefaultY) {
		vy = BRICK_DEFLECT_SPEED;
	}
	else {
		vy = 0;
	}
	y += dy;
	if (y > positionDefaultY) y = positionDefaultY;

	if (this->isEmpty == false && state == BRICK_STATE_EMPTY) {
		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Mario *mario = ((PlayScene*)scene)->GetPlayer();

		GameObject *bonus = new Bonus();
		bonus->SetPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y - BONUS_BBOX_HEIGHT);
		if (mario->nx > 0) {
			bonus->nx = 1;
		}
		else {
			bonus->nx = -1;
		}

		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			bonus->SetState(BONUS_STATE_MUSHROOM_RED);
		}
		else {
			bonus->SetState(BONUS_STATE_LEAF);
		}
		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(BONUS_ANIMATION_SET_ID);

		bonus->SetAnimationSet(ani_set);

		((PlayScene*)scene)->pushObject(bonus);
		isEmpty = true;
	}
}

void BrickButton::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);

	if (y > positionDefaultY) y = positionDefaultY;

	if (this->isEmpty == false && state == BRICK_STATE_EMPTY) {
		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Mario *mario = ((PlayScene*)scene)->GetPlayer();

		GameObject *button = new Button(x, y);
		button->SetPosition(this->x, this->y - BONUS_BBOX_HEIGHT);

		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(BUTTON_ANIMATION_SET_ID);

		button->SetAnimationSet(ani_set);

		((PlayScene*)scene)->pushObject(button);
		isEmpty = true;
	}
}

void BrickQuestionCoinPermanent::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	GameObject::Update(dt);

	if (this->y < positionDefaultY) {
		vy = BRICK_DEFLECT_SPEED;
	}
	else {
		vy = 0;
	}
	y += dy;

	//vector<LPCollisionEvent> coEvents;
	//vector<LPCollisionEvent> coEventsResult;

	//coEvents.clear();

	//CalcPotentialCollisions(coObjects, coEvents);

	//// No collision occured, proceed normally
	//if (coEvents.size() != 0)
	//{
	//	float min_tx, min_ty, nx = 0, ny = 0;
	//	float rdx = 0;
	//	float rdy = 0;
	//	//// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	// block every object first!
	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCollisionEvent e = coEventsResult[i];

	//		//if (dynamic_cast<Koopa *>(e->obj)) // if e->obj is Koopa 
	//		//{
	//		//	status = 1;
	//		//	Koopa *koopa = dynamic_cast<Koopa *>(e->obj);
	//		//	if (state == BRICK_STATE_EMPTY && status == 1) {
	//		//		if (koopa->state != KOOPA_STATE_DIE) {
	//		//			koopa->SetState(KOOPA_STATE_DIE);
	//		//		}
	//		//	}
	//		//}

	//	}
	//}
	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (y > positionDefaultY) y = positionDefaultY;

	if (this->isEmpty == false && state == BRICK_STATE_EMPTY) {

		Game *game = Game::GetInstance();
		LPScene scene = Game::GetInstance()->GetCurrentScene();
		Mario *mario = ((PlayScene*)scene)->GetPlayer();

		GameObject *coin = new Coin(this->x + BRICK_BBOX_WIDTH / 2, this->y - COIN_BBOX_WIDTH);
		coin->SetPosition(this->x + BRICK_BBOX_WIDTH / 4, this->y - COIN_BBOX_WIDTH);
		coin->SetState(COIN_STATE_EARN);

		LPAnimation_Set ani_set = AnimationSets::GetInstance()->Get(COIN_ANIMATION_SET_ID);

		coin->SetAnimationSet(ani_set);

		((PlayScene*)scene)->pushObject(coin);
		mario->setScore(mario->getScore() + SCORE_PLUS);
		mario->setPoint(mario->getPoint() + 1);
		state = BRICK_STATE_NORMAL;
	}


}