#include "Vector2D.h"


Vector2D::~Vector2D()
{
}


inline Vector2D operator*(float scalar, const Vector2D& vector)
{
	auto retVec = vector.vec;
	for (auto& x : retVec)
	{
		x *= scalar;
	}
	
	return Vector2D(retVec);
}