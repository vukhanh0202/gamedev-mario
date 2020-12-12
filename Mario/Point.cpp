#include "Point.h"


void Point::Render()
{
	int ani = POINT_ANI_0;
	switch (state) {
	case 0: ani = POINT_ANI_0; break;
	case 1: ani = POINT_ANI_1; break;
	case 2: ani = POINT_ANI_2; break;
	case 3: ani = POINT_ANI_3; break;
	case 4: ani = POINT_ANI_4; break;
	case 5: ani = POINT_ANI_5; break;
	case 6: ani = POINT_ANI_6; break;
	case 7: ani = POINT_ANI_7; break;
	case 8: ani = POINT_ANI_8; break;
	case 9: ani = POINT_ANI_9; break;
	}

	animation_set->at(ani)->Render(x, y);
}

void Point::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;

}

