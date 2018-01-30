#pragma once
#include <vector>

class Vector2D
{
public:

	Vector2D(const std::vector<float>& vec) : vec(vec){}
	~Vector2D();
	inline friend Vector2D operator*(float scalar, const Vector2D& vector);
private:
	std::vector<float> vec;
};