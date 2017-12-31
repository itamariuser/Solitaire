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

bool ClassRenderer::renderLineColored(const Point& start, const Point& end, const Color& c)
{
	Color oldColor = getRenderColor();
	
	setRenderColor(c);
	// Draw a line 
	//--- 
	int ret =
		SDL_RenderDrawLine(
			renderer,
			start.x,     
			start.y,     
			end.x,       
			end.y); 
	setRenderColor(oldColor);
	if (ret != 0)
	{
		const char *error = SDL_GetError();
		if (*error != '\0')
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not renderDrawLineColored. SDL Error: %s at line #%d of file %s/n", error, __LINE__, __FILE__);
			SDL_ClearError();
		}
		return false;
	}
	return true;
}

void ClassRenderer::renderImage(const SDL_Texture * const texture, const Shapes::Rect * const rect)
{
	SDL_RenderCopy(renderer, const_cast<SDL_Texture*>(texture), nullptr, &SDL_Rect(*rect));
}
