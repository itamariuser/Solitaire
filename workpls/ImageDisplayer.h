//#pragma once
//#include "Shapes.h"
//#include "classRenderer.h"
//#include "Graphic.h"
//
//
//
//class ImageDisplayer
//{
//public:
//	
//	~ImageDisplayer()
//	{
//		delete instance;
//	}
//	static ImageDisplayer* getInstance()
//	{
//		if (!instance)
//		{
//			instance = new ImageDisplayer();
//		}
//		return instance;
//	}
//	Point getImageSize(char* imagePath, Uint32* format = nullptr, int* access = nullptr) const throw(int);
//	Point getImageSize(SDL_Texture* image, Uint32* format = nullptr, int* access = nullptr) const throw(int);
//	SDL_Texture* getTexture(char* imagePath);
//	void renderImage(const Texture texture);
//	void renderImage(char* imagePath, Shapes::Rect* rect = nullptr);
//private:
//	static ImageDisplayer* instance;
//	ClassRenderer* ren;
//	std::vector<char*> imagePaths;
//	std::unordered_map< char*, SDL_Texture*> loadedImages;
//	void ctor_init_imagePaths();
//	void loadImages();
//	ImageDisplayer();
//	
//	
//};
//
