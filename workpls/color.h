#pragma once

#include "SDL.h"//


class Color : public SDL_Color
{
public:
	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
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
};

