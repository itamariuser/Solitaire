#include "Physical.h"



Physical::Physical(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture,
	Point sizeSpeed, int width, int height, float mass, const Point& drag)
: Texture(name, center, speed, gView, texture, sizeSpeed, width, height), mass(mass), drag(drag)
{
	
}


Physical::~Physical()
{
}

void Physical::next()
{
	Texture::next();
	/*if (speed.x >= drag.x)
		speed.x -= drag.x;
	

	if (speed.y >= drag.y)
		speed.y -= drag.y;*/
	
}
