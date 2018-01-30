#pragma once
#include "GameView.h"


class GameView;
class Physical;



class PhysicsEngine
{
public:
	void collide(const std::shared_ptr<Physical> phys1, const std::shared_ptr<Physical> phys2);
	PhysicsEngine();
	~PhysicsEngine();
};

