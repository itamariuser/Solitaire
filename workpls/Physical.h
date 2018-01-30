#pragma once
#include "Graphic.h"
class Physical : public Texture
{
public:
	Physical(std::string name, Point center, Point speed, GameView* gView, SDL_Texture* texture = nullptr, Point sizeSpeed = { 0,0 }, int width = -1, int height = -1, float mass = 1, const Point& drag = {0,0});

	~Physical();
	float getMass() const { return mass; }
	void setMass(float m) { mass = m; }

	virtual void next();
private:
	float mass;
	Point lastPt;
	Point drag;
};

