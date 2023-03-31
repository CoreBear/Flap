#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

class Food final : public SceneObject
{
public:
	// Initialization
	// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
	Food() : SceneObject(new Structure::CollisionRenderInfo(Enums::ObjectType::Food, m_position)) { return; }

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject) final;
};

#endif FOOD_H