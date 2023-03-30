#ifndef BIRD_H
#define BIRD_H

#include "SceneObject.h"
#include "Structure.h"

class Bird final : public SceneObject
{
public:
	// Initialization
	Bird();
	void Initialize(const Structure::Generic& _genericContainer) override;

	// Updates
	void FixedUpdate() override;

private:
	// Static Variables
	const static float s_flyForwardSpeed;

	// Member Variables
	Structure::Vector2<float> m_dragVelocity;
	Structure::Vector2<float> m_velocity;
};

#endif BIRD_H