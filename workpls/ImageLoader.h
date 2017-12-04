#pragma once
#include "Shapes.h"
#include "classRenderer.h"
class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();
	static void loadImages(ClassRenderer rend);
};

