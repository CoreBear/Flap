#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

class Food final : public SceneObject
{
public:
	// Initialization
	Food() : SceneObject(Enums::ObjectType::Food) { return; }

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject) final;
};

#endif FOOD_H