#pragma once

class Point {
public:
	float x;
	float y;
	template<class ConvertibleToFloat0, class ConvertibleToFloat1>
	Point(ConvertibleToFloat0 x, ConvertibleToFloat1 y) : x(x), y(y)
	{
		int i = 0;
	}
	Point() : x(0.0f), y(0.0f) {}

	virtual double distanceFromPoint(Point p);
	Point operator+(const Point& other) const { return Point(this->x + other.x,this->y+other.y); }
	void operator+=(const Point& other){ this->x += other.x; this->y += other.y; }
	bool operator==(const Point& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Point& other) const { return !(*this == other); }
	void operator*=(float multiplier) { this->x *= multiplier; this->y *= multiplier; }
	Point operator*(float multiplier) const{ return Point(x*multiplier, y*multiplier); }
	/*inline friend Point operator*(float scalar, const Point& point);*/
	bool isZero() const { return x == 0 && y == 0; }
};