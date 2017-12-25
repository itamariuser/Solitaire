#include "ImageLoader.h"
#include <filesystem>
#include <iostream>
#include <sstream>
using namespace std::experimental::filesystem;
void ImageLoader::loadImages(const std::string& imageFolder, std::unordered_map<char*, SDL_Texture*>& imagesMap, std::function<SDL_Texture* (const char* path)> loadFunction)
{
	for(auto& p : directory_iterator(imageFolder))
	{
		if (!is_directory(p))
		{
			std::stringstream ss;
			ss << p;
			std::string s;
			ss >> s;
			
			auto sub = s.substr(s.find_last_of("\\")+1);
			static auto name = const_cast<char*>(sub.c_str());
			static auto entry = const_cast<char*>(s.c_str());
			imagesMap[name] = loadFunction(entry);
			//imagesMap[const_cast<char*>(s.c_str())] = loadFunction(s.c_str());
			int i = 0;
		}
	}
	
}


/*std::stringstream ss;
ss << p;
std::string s;
ss >> s;
auto pos = s.find_last_of("\\");
char* name = const_cast<char*>(s.substr(pos).c_str());
imagesMap[name] = loadFunction(name);*/