#ifndef BIRD_H
#define BIRD_H

#include "Enums.h"
#include "InputReceiver.h"
#include "SceneObject.h"
#include "Structure.h"

#include <list>

class Snake : public SceneObject
{
public:
	// Member Variables
	std::list<Structure::Vector2<int>> m_listOfBodyPositions;
	std::list<Structure::Vector2<int>>::iterator m_headTraversingIterator;
	std::list<Structure::Vector2<int>>::iterator m_tailTraversingIterator;
	std::list<Structure::Vector2<int>>::iterator m_tailIterator;

	// Initialization
	void Initialize(const Structure::Generic& _genericContainer) override;
	Snake();

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
	Structure::Vector2<int> m_newBodyPosition;
	Structure::Vector2<float> m_realHeadPosition;
	const static Structure::Vector2<float> s_velocity;

	// Functionality
	void AddTail();
	void HandleInput();
	void Move();
};

#endif BIRD_H