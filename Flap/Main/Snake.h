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
	Snake() : SceneObject(Enums::ObjectType::Snake), m_currentDirection(Enums::Direction::NA), m_newDirection(Enums::Direction::NA) { return; }

	// Updates
	void FixedUpdate() override final;

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject) final;

protected:
	// Member Variables

	// Functionality
	void BeginDown() { m_newDirection = Enums::Direction::Down; }
	void BeginLeft() { m_newDirection = Enums::Direction::Left; }
	void BeginRight() { m_newDirection = Enums::Direction::Right; }
	void BeginUp() { m_newDirection = Enums::Direction::Up; }

private:
	// Static Variables
	Enums::Direction m_currentDirection;
	Enums::Direction m_newDirection;
	const static Structure::Vector2<float> s_velocity;

	// Functionality
	void HandleTurn();
	void Move();
};

#endif BIRD_H