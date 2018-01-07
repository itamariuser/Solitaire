#pragma once
#include "SDL.h"


class Color : public SDL_Color
{
public:
	Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 0)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
	Color operator+(const Color& other) const
	{
		Uint8 red = r + other.r;
		Uint8 green = g + other.g;
		Uint8 blue = b + other.b;
		Uint8 alpha = a + other.a;
		return { red, green, blue, alpha }; 
	}

	bool operator!=(const Color& other) const
	{
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}
};

