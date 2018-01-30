#include "PhysicsEngine.h"
#include "Graphic.h"
#include "Physical.h"


void PhysicsEngine::collide(const std::shared_ptr<Physical> phys1, const std::shared_ptr<Physical> phys2)
{
	const auto m1 = phys1->getMass(), m2 = phys2->getMass();
	const auto u1 = phys1->getSpeed(), u2 = phys2->getSpeed();
	const auto m1_plus_m2 = m1 + m2;
	const auto first = (m1 - m2) / m1_plus_m2;
	const auto z1 = u1 * first;
	const auto f1 = (2 * m2 / m1_plus_m2);
	const auto s1 = z1 + u2 *  f1;
	const auto z2 = u2 * -first;
	const auto f2 = (2 * m1 / m1_plus_m2);
	const auto s2 = z2 + u1 * f2;
	phys1->setSpeed(s1 );

	phys2->setSpeed(s2 );

	int i = 0;
}

PhysicsEngine::PhysicsEngine()
{
}


PhysicsEngine::~PhysicsEngine()
{
}
