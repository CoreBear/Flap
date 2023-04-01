#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

class Food final : public SceneObject
{
public:
	// Initialization
	Food();
	void Initialize(const Structure::Generic& _genericContainer) override final;

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject) final;

private:
	// Functionality
	void UpdateValue(int _value);
};

#endif FOOD_H