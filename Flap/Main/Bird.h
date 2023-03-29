#ifndef BIRD_H
#define BIRD_H

#include "SceneObject.h"
#include "Structure.h"

class Bird : public SceneObject
{
public:
	// Initialization
	Bird();
	void Initialize(const Structure::Generic& _genericContainer) override;

	// Updates
	void FixedUpdate() override;

private:
	// Member Variables
	Structure::Vector2<int> m_velocity;

	// Static Variables
	const static float s_flyForwardSpeed;
};

#endif BIRD_H