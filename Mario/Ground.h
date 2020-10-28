#include "GameObject.h"

#define GROUND_BBOX_WIDTH  16
#define GROUND_BBOX_HEIGHT 16

class Ground : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};