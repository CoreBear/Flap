#ifndef FOOD_H
#define FOOD_H

#include "SceneObject.h"

class Food final : public SceneObject
{
public:
	// Initialization
	Food();
	Food(const Food&) = delete;
	Food& operator=(const Food&) = delete;
	void Initialize(const Structure::Generic* const _genericContainer) override;

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject, const Structure::Vector2& _collisionCellCR);

private:
	// Functionality
	void UpdateValue(int _value);
};

#endif FOOD_H