#include "GameObject.h"

#define GROUND_BBOX_WIDTH  16
#define GROUND_BBOX_HEIGHT 16

#define OBJECT_TYPE_GROUND	0

class Ground : public GameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetTypeObject() {
		return OBJECT_TYPE_GROUND;
	}
};