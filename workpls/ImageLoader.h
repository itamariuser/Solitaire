#pragma once
#include <string>
#include <SDL.h>
#include <unordered_map>
#include <functional>


class ImageLoader
{
public:
	static void loadImages(const std::string& imageFolder, std::unordered_map < std::string, SDL_Texture* > & imagesMap, std::function<SDL_Texture* (const char* path)> loadFunction);
};
