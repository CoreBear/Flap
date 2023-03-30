#ifndef BIRD_H
#define BIRD_H

#include "Enums.h"
#include "InputReceiver.h"
#include "SceneObject.h"
#include "Structure.h"

class Snake : public SceneObject
{
public:
	// Initialization
	void Initialize(const Structure::Generic& _genericContainer) override;

	// Updates
	void FixedUpdate() override final;

protected:
	// Member Variables
	Enums::Direction m_moveDirection;

private:
	// Static Variables
	const static float s_moveSpeed;
};

#endif BIRD_H