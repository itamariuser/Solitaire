#pragma once

class Point {
public:
	int x;
	int y;
	virtual double distanceFromPoint(Point p);

	Point(int x, int y);
	Point() : x(0), y(0) {}

	Point operator+(const Point& other) { return Point(this->x + other.x,this->y+other.y); }
	void operator+=(const Point& other){ this->x += other.x; this->y += other.y; }
	bool operator==(const Point& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Point& other) const { return !(*this == other); }
	void operator*=(float multiplier) { this->x *= multiplier; this->y *= multiplier; }
	Point operator*(float multiplier) const { return Point( x*multiplier,y*multiplier); }
	/*Point operator-(const Point& other) const { Point z{ x - other.x, y - other.y};
	return z; }*/
	bool isZero() const { return x == 0 && y == 0; }
};//