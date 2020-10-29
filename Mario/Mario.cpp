#include "Mario.h"
#include "Goomba.h"
#include "Brick.h"
#include "Box.h"
#include "Koopa.h"

Mario::Mario(float x, float y) : GameObject()
{
	level = MARIO_LEVEL_SUPER_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

	lastJumpTime = 0;
	ny = -1;
	hold = false;
	hit = false;
}

void Mario::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCollisionEvent> coEvents;
	vector<LPCollisionEvent> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		isCollision = true;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCollisionEvent e = coEventsResult[i];

			if (dynamic_cast<Goomba *>(e->obj)) // if e->obj is Goomba 
			{
				Goomba *goomba = dynamic_cast<Goomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								//level = MARIO_LEVEL_SMALL;
								level--;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			else if (dynamic_cast<Koopa *>(e->obj)) // if e->obj is Koopa 
			{
				Koopa *koopa = dynamic_cast<Koopa *>(e->obj);

				// jump on top >> kill Koopa
				if (e->ny < 0)
				{
					if (koopa->GetState() != KOOPA_STATE_DIE)
					{
						koopa->SetState(KOOPA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx > 0) // collision right to left
				{
					// Koopa die
					if (koopa->GetState() == KOOPA_STATE_DIE)
					{
						// Hold koopa
						if (this->hold == hold)
						{
							if (this->level == MARIO_LEVEL_SUPER_BIG)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH + 2, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_BIG)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH + 2, y + MARIO_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_SMALL)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH + 2, y - 2);
							}
							koopa->SetState(KOOPA_STATE_HOLDING);
						}
						else
						{
							// Hit Koopa
							this->SetState(MARIO_STATE_HIT);
							koopa->SetState(KOOPA_STATE_THROWING_LEFT);
						}
					}
					else if (koopa->GetState() == KOOPA_STATE_HOLDING)
					{
						if (this->hold == true)
						{
							vx = -MARIO_WALKING_SPEED;
							x += dx;
							if (this->level == MARIO_LEVEL_SUPER_BIG)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH_HOLD - 1, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_BIG)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH_HOLD - 1, y + MARIO_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_SMALL)
							{
								koopa->SetPosition(x - KOOPA_BBOX_WIDTH_HOLD - 1, y - 2);
							}
						}
						else
						{
							koopa->SetState(KOOPA_STATE_THROWING_LEFT);
						}
						
					}
					else
					{
						// koopa live
						if (untouchable == 0)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level--;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				} // end collision right->left
				else if (e->nx < 0) // collision left->right
				{
					if (koopa->GetState() == KOOPA_STATE_DIE)
					{
						// Hold koopa
						if (this->hold == true)
						{
							if (this->level == MARIO_LEVEL_SUPER_BIG)
							{
								koopa->SetPosition(x + MARIO_SUPER_BIG_BBOX_WIDTH, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_BIG)
							{
								koopa->SetPosition(x + MARIO_BIG_BBOX_WIDTH, y + MARIO_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_SMALL)
							{
								koopa->SetPosition(x + MARIO_SMALL_BBOX_WIDTH, y - 2);
							}
							koopa->SetState(KOOPA_STATE_HOLDING);
						}
						else
						{
							this->SetState(MARIO_STATE_HIT);
							koopa->SetState(KOOPA_STATE_THROWING_RIGHT);
						}

					}
					else if (koopa->GetState() == KOOPA_STATE_HOLDING)
					{
						if (this->hold == true)
						{
							vx = MARIO_WALKING_SPEED;
							x += dx;
							if (this->level == MARIO_LEVEL_SUPER_BIG)
							{
								koopa->SetPosition(x + MARIO_SUPER_BIG_BBOX_WIDTH - 2, y + MARIO_SUPER_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_BIG)
							{
								koopa->SetPosition(x + MARIO_BIG_BBOX_WIDTH - 2, y + MARIO_BIG_BBOX_HEIGHT / 4);
							}
							else if (this->level == MARIO_LEVEL_SMALL)
							{
								koopa->SetPosition(x + MARIO_SMALL_BBOX_WIDTH - 2, y - 2);
							}
						}
						else
						{
							koopa->SetState(KOOPA_STATE_THROWING_RIGHT);
						}
						
					}
					else
					{
						if (untouchable == 0)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level--;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}// Colision left to right
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
	{
		ani = MARIO_ANI_DIE;
	}
	else {
		if (level == MARIO_LEVEL_BIG)
		{
			if (hold == true)
			{
				if (vx == 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
					}
				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_BIG_HOLD_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_HOLD_LEFT;
				}
			}
			else if (hit == true)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_BIG_HIT_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_HIT_LEFT;
				}
			}
			else
			{
				if (vx == 0)
				{
					if (ny > 0) // sit
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_BIG_SITTING_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_BIG_SITTING_LEFT;
						}
					}
					else // not sit
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_BIG_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_BIG_IDLE_LEFT;
						}
					}

				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
			}

		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (hold == true)
			{
				if (vx == 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
					}
				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_SMALL_HOLD_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_HOLD_LEFT;
				}
			}
			else if (hit == true)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_SMALL_HIT_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_HIT_LEFT;
				}
			}
			else
			{
				if (vx == 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SMALL_IDLE_LEFT;
					}
				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
			}

		}
		else if (level == MARIO_LEVEL_SUPER_BIG)
		{
			if (hold == true)
			{
				if (vx == 0)
				{
					if (nx > 0)
					{
						ani = MARIO_ANI_SUPER_BIG_IDLE_HOLD_RIGHT;
					}
					else
					{
						ani = MARIO_ANI_SUPER_BIG_IDLE_HOLD_LEFT;
					}
				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_SUPER_BIG_HOLD_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SUPER_BIG_HOLD_LEFT;
				}
			}
			else if (hit == true)
			{
				if (nx > 0)
				{
					ani = MARIO_ANI_SUPER_BIG_HIT_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SUPER_BIG_HIT_LEFT;
				}
			}
			else {
				if (vx == 0)
				{
					if (ny > 0) // sit
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SUPER_BIG_SITTING_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SUPER_BIG_SITTING_LEFT;
						}
					}
					else // not sit
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_SUPER_BIG_IDLE_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_SUPER_BIG_IDLE_LEFT;
						}
					}
				}
				else if (vx > 0)
				{
					ani = MARIO_ANI_SUPER_BIG_WALKING_RIGHT;
				}
				else
				{
					ani = MARIO_ANI_SUPER_BIG_WALKING_LEFT;
				}
			}

		}
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	// To see BoundingBox
	//RenderBoundingBox();
}

void Mario::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;// Not sit
		hit = false; // cancel state hit
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;
		hit = false; // cancel state hit
		break;
	case MARIO_STATE_JUMP:
		if (ny == -1) {
			DWORD t = GetTickCount() - lastJumpTime;
			// 0.5s sleep when mario jump
			if (t > 500 && isCollision == true)
			{

				lastJumpTime = GetTickCount();
				isCollision = false;
				vy = -MARIO_JUMP_SPEED_Y;
				ny = -1;
			}
		}
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		if (ny == 1)
		{
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			this->y -= (b - t);
		}
		ny = -1;
		//hold = false;
		//hit = false; // cancel state hit
		break;
	case MARIO_STATE_SITTING_RIGHT:
		vx = 0;
		ny = 1;
		break;
	case MARIO_STATE_SITTING_LEFT:
		vx = 0;
		ny = 1;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	/*case MARIO_STATE_HOLD:
		hold = true;
		hit = false;
		ny = -1;
		break;
	case MARIO_STATE_UNHOLD:
		hold = false;
		ny = -1;
		break;*/
	case MARIO_STATE_HIT:
		hold = false;
		hit = true;
		vx = 0;
		ny = -1;
		break;
	}
}

void Mario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		if (ny > 0 && hit == false && hold == false)
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT_SITTING;
		}
		else
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		right = x + MARIO_BIG_BBOX_WIDTH;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_SUPER_BIG)
	{
		if (ny > 0 && hit == false && hold == false)
		{
			bottom = y + MARIO_SUPER_BIG_BBOX_HEIGHT_SITTING;
		}
		else
		{
			bottom = y + MARIO_SUPER_BIG_BBOX_HEIGHT;
		}
		right = x + MARIO_SUPER_BIG_BBOX_WIDTH;
	}

	if (hold == true)
	{
		right -= 3;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void Mario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SUPER_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Mario::UpLevel()
{
	if (level != MARIO_LEVEL_SUPER_BIG) {
		int l = ++level;
		SetPosition(x, y - MARIO_SUPER_BIG_BBOX_HEIGHT);
		SetLevel(l);
	}
}

void Mario::DownLevel()
{
	if (level != MARIO_LEVEL_SMALL) {
		int l = --level;
		SetLevel(l);
	}
}



