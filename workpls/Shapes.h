#pragma once
#include "point.h"
#include <SDL.h>

namespace Shapes
{
	class Circle {
	public:
		Circle(Point c, float r) : center(c), radius(r) {};
		Point center;
		float radius;
	};

	class Rect {
	public:
		Rect(int x, int y, int w, int h) : center(x, y), w(w), h(h) {}
		Rect(SDL_Rect rect) : center(rect.x, rect.y), w(rect.w), h(rect.h) {}
		operator SDL_Rect() const { SDL_Rect retRect; retRect.x = center.x; retRect.y = center.y; retRect.w = w; retRect.h = h; return retRect; }
		Point center;
		int w, h;
	};
}//