#include "Box.h"

void Box::Render()
{
	animation_set->at(0)->Render(x, y);
}
void Box::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	/*if (state == BOX_STATE_ENABLE)
	{
		l = x;
		t = y;
		r = x + BOX_BBOX_WIDTH;
		b = y + BOX_BBOX_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + BOX_BBOX_WIDTH;
		b = y + BOX_BBOX_HEIGHT;
	}*/
	l = 0;
	t = 0;
	r = 0 + BOX_BBOX_WIDTH;
	b = 0 + BOX_BBOX_HEIGHT;
}
void Box::Update(DWORD dt, vector<LPGameObject> *coObjects)
{


	//// Calculate dx, dy 
	//GameObject::Update(dt, coObjects);

	//static float yObj = -1;

	//static boolean flag = false;

	//LPScene scene = Game::GetInstance()->GetCurrentScene();
	//Mario *mario = ((PlayScene*)scene)->GetPlayer();
	//float xMario, yMario;
	//mario->GetPosition(xMario, yMario);

	//// If mario jump on box-> enable box
	////if (((xMario + MARIO_SUPER_BIG_BBOX_WIDTH) > this->x && (xMario - MARIO_SUPER_BIG_BBOX_WIDTH < this->x + BOX_BBOX_WIDTH))) {
	//if (x <= xMario && ((x + BOX_BBOX_WIDTH) >= xMario)) {
	//	if (flag == false)
	//	{
	//		if (status == false && (x <= xMario && (x + BOX_BBOX_WIDTH) >= xMario))
	//		{
	//			yObj = y;
	//			status = true;
	//		}
	//		else if (status == true && y == yObj && state == BOX_STATE_DISABLE) {
	//			flag = true;
	//		}
	//	}
	//	if (flag == true && yMario + MARIO_SUPER_BIG_BBOX_HEIGHT <= yObj)
	//	{
	//		state = BOX_STATE_ENABLE;
	//		flag = false;
	//		yObj = -1;
	//	}
	//}
	//else {
	//	status = false;
	//	state = BOX_STATE_DISABLE;
	//}

}


// BOX (Merge box)
void Boxs::PushBox(Box* box)
{
	listBox.push_back(box);
}

void Boxs::Render()
{
	for (size_t i = 0; i < listBox.size(); i++)
	{
		listBox[i]->Render();
	}
}

void Boxs::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y;
}
void Boxs::Update(DWORD dt, vector<LPGameObject> *coObjects)
{

}


