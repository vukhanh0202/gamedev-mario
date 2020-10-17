#include "GameObject.h"
#include "Debug.h"

/*
	Initialize value for GameObject
*/
GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
	
}
/*
	Destructor
*/
GameObject::~GameObject()
{

}

/*
	Update distance object
*/
void GameObject::Update(DWORD dt, vector<LPGameObject> *coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

void GameObject::AddAnimation(int aniId)
{
	LPAnimation ani = Animations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}