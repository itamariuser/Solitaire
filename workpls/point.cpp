#include <math.h>
#include "point.h"


	double Point::distanceFromPoint(Point p) {
		return sqrt(pow(p.x-x,2)+pow(p.y - y, 2));
	}

	Point::Point(int x, int y)
	{
		Point::x = x;
		Point::y = y;
	}