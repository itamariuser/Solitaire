#pragma once
#include <SDL.h>
#include "color.h"
#include <vector>
#include <unordered_map>
#include "point.h"


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

	ClassRenderer(SDL_Window* win, int index, Uint32 flags);

	Color setRenderColor(Color color);
	Color getRenderColor();

	void clear();
	void present();

	Point ClassRenderer::getImageSize(char* imagePath, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	Point ClassRenderer::getImageSize(SDL_Texture* image, Uint32* format = nullptr, int* access = nullptr) const throw(int);
	SDL_Texture* getTexture(char* imagePath);
	void ClassRenderer::renderImage(const Texture texture);
	void renderImage(char* imagePath, Shapes::Rect* rect = nullptr);
private:
	std::vector<char*> imagePaths;
	std::unordered_map< char* , SDL_Texture*> loadedImages;
	void ctor_init_imagePaths();
	void loadImages();
};
