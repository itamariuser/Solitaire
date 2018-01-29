#pragma once
#include <SDL.h>
#include "color.h"
#include "point.h"
#include "Window.h"
#include "Shapes.h"
class Texture;

class ClassRenderer
{
public:
	SDL_Renderer* renderer;
	
	ClassRenderer(Window& win, Uint32 flags, int index = -1);

	Color setRenderColor(Color color);
	Color getRenderColor() const;

	void clear();
	void present();

	bool renderLineColored(const Point& start, const Point& end, const Color& c, int thickness = 1);
	void renderImage(const SDL_Texture* const texture, const Shapes::Rect* const rect);
protected:
	Color renderColor;
};
