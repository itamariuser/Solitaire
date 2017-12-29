#include <SDL.h>
#include "classRenderer.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Shapes.h"
#include "graphic.h"


ClassRenderer::ClassRenderer(Window& win, Uint32 flags, int index) : renderColor(0,0,0,0)
{
	renderer = SDL_CreateRenderer(win.getSDL_Window(), index, flags);
}


Color ClassRenderer::setRenderColor(Color color)
{
	Color lastColor = renderColor;
	renderColor = color;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	return lastColor;
}

Color ClassRenderer::getRenderColor() const
{
	return renderColor;
}

void ClassRenderer::clear()
{
	SDL_RenderClear(renderer);
}

void ClassRenderer::present()
{
	SDL_RenderPresent(renderer);
}