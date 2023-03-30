#ifndef BIRD_H
#define BIRD_H

#include "Enums.h"
#include "InputReceiver.h"
#include "SceneObject.h"
#include "Structure.h"

class Bird final : public SceneObject, InputReceiver
{
public:
	// Initialization
	Bird();
	void Initialize(const Structure::Generic& _genericContainer) override;

	// Updates
	void FixedUpdate() override final;
	void Update() override final;

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override final;
	void InputLeft(Enums::InputPressState _inputPressState) override final;
	void InputRight(Enums::InputPressState _inputPressState) override final;
	void InputUp(Enums::InputPressState _inputPressState) override final;

private:
	// Static Variables
	const static float s_upForce;
	const static float s_flyForwardSpeed;

	// Member Variables
	// HACK:
	bool m_isAPlayer;
	Structure::Vector2<float> m_dragVelocity;
	Structure::Vector2<float> m_velocity;
};

#endif BIRD_H