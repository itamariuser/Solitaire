#pragma once
#include <SDL.h>
#include <string>
#include "point.h"
class Window
{
public:
	Window(std::string title, const Point& position, const Point& dimensions, Uint32 flags) : title(title), position(position), dimensions(dimensions), flags(flags)
	{
		baseWindow = SDL_CreateWindow(title.c_str(), position.x, position.y,dimensions.x,dimensions.y,flags);
	}
	SDL_Window* const getSDL_Window() { return baseWindow; }
	const Point getDimensions() const { return dimensions; }
	void setTitle(const std::string& newTitle) {
		title = newTitle; 
		SDL_SetWindowTitle(baseWindow, newTitle.c_str());
	}
private:
	SDL_Window* baseWindow;
	std::string title;
	Point position;
	Point dimensions;
	Uint32 flags;

};
