#pragma once
#include <string>
#include <SDL.h>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <iostream>
#include <sstream>

using namespace std::experimental::filesystem;

class ResourceLoader
{
public:
	template<class Resource, class ResourcePtr>
	static void load(const std::string& imageFolder, std::unordered_map < std::string, ResourcePtr >& imagesMap, std::function<Resource* (std::string path)> loadFunction)
	{
		for (auto& p : directory_iterator(imageFolder))
		{
			if (!is_directory(p))
			{
				std::stringstream ss;
				ss << p;
				std::string s;
				ss >> s;

				auto sub = s.substr(s.find_last_of("\\") + 1);
				auto name = const_cast<char*>(sub.c_str());
				auto entry = const_cast<char*>(s.c_str());
				imagesMap[name] = ResourcePtr(loadFunction(entry));
			}
		}
	}
};
