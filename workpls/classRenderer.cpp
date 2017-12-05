#include <SDL.h>
#include "classRenderer.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Shapes.h"
#include "graphic.h"


ClassRenderer::ClassRenderer(SDL_Window* win, int index, Uint32 flags):renderColor(0,0,0,0)
{
	renderer = SDL_CreateRenderer(win, index, flags);
	loadImages();
}


Color ClassRenderer::setRenderColor(Color color)
{
	Color lastColor = renderColor;
	renderColor = color;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	return lastColor;
}

Color ClassRenderer::getRenderColor()
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

void ClassRenderer::loadImages()
{
	ctor_init_imagePaths();
	for(char* path : imagePaths)
	{
		auto z = IMG_LoadTexture(renderer, path);
		loadedImages[path] = IMG_LoadTexture(renderer, path);
	}
}

void ClassRenderer::ctor_init_imagePaths()
{
	imagePaths.push_back("assets/KingLeaf.png");
}

Point ClassRenderer::getImageSize(char* imagePath, Uint32* format, int* access) const throw(int)
{
	if(loadedImages.find(imagePath) == loadedImages.end())
	{
		std::cerr << "No such image loaded, add it to imagePaths.\n";
		throw(-1);
	}
	Point retPt;
	SDL_QueryTexture(loadedImages.at(imagePath), format, access, &retPt.x,&retPt.y);
	return retPt;
}

Point ClassRenderer::getImageSize(SDL_Texture* image, Uint32* format, int* access) const throw(int)
{
	if (!image)
	{
		std::cerr << "No image to load.\n";
		throw(-1);
	}
	Point retPt;
	SDL_QueryTexture(image, format, access, &retPt.x, &retPt.y);
	return retPt;
}

SDL_Texture* ClassRenderer::getTexture(char* imagePath)
{
	if (loadedImages.find(imagePath) == loadedImages.end())
	{
		std::cerr << "No such image loaded, add it to imagePaths.\n";
		throw(-1);
	}

	return loadedImages[imagePath];
}

void ClassRenderer::renderImage( const Texture texture)
{
	auto rect = SDL_Rect(texture.getRenderRect());
	auto* texr = texture.getTexture();
	SDL_RenderCopy(renderer, const_cast<SDL_Texture*>(texr), nullptr, &rect);
}

void ClassRenderer::renderImage(char* imagePath, Shapes::Rect* rect)
{
	auto texture = getTexture(imagePath);
	renderImage()
}