#pragma once
#include <SDL.h>
#include "color.h"
#include <vector>
#include <unordered_map>
#include "point.h"
#include "Window.h"

	class Graphic;
	class Line;
	class SimpleLine;
	class Circle;
	class Diamond;
	class Texture;
	class Card;

namespace Shapes {
	class Rect;
	class Circle;
}

class ClassRenderer
{
public:
	SDL_Renderer* renderer;
	Color renderColor;

	ClassRenderer(Window& win, Uint32 flags, int index = -1);

	Color setRenderColor(Color color);
	Color getRenderColor() const;

	void clear();
	void present();//

private:
};
