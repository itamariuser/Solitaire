#include <math.h>
#include "point.h"


	double Point::distanceFromPoint(Point p) {
		return sqrt(pow(p.x-x,2)+pow(p.y - y, 2));
	}

	//inline Point operator*(float scalar, const Point& point)
	//{
	//	return Point(point.x * scalar, point.x * scalar);
	//}
