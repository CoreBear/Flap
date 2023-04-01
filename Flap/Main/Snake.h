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
	// Initialization
	void Initialize(const Structure::Generic& _genericContainer) override;
	Snake();

	// Updates
	void FixedUpdate() override final;

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject, const Structure::Vector2& _collisionCellCR) final;

protected:
	// Member Variables

	// Functionality
	inline void BeginDown() { m_newDirection = Enums::Direction::Down; }
	inline void BeginLeft() { m_newDirection = Enums::Direction::Left; }
	inline void BeginRight() { m_newDirection = Enums::Direction::Right; }
	inline void BeginUp() { m_newDirection = Enums::Direction::Up; }

private:
	// Member Variables
	Enums::Direction m_currentDirection;
	Enums::Direction m_newDirection;
	const Structure::Generic* mp_collisionPackage;
	int m_moveTargetFrame;
	int m_numberOfTailSectionsToAdd;
	std::list<Structure::Vector2> m_listOfBodyPositions;
	std::list<Structure::Vector2>::iterator m_headTraversingIterator;
	std::list<Structure::Vector2>::iterator m_tailTraversingIterator;
	std::list<Structure::Vector2>::iterator m_tailIterator;
	unsigned int m_numberOfFramesPerCell;
	unsigned int m_numberOfFramesPerCellHorizontal;
	unsigned int m_numberOfFramesPerCellVertical;
	Structure::Vector2 m_newTailPosition;

	// Static Variables

	// Functionality
	void Death();
	void HandleInput();
	void HorizontalTurn();
	void Move();
	void Turn();
	void UpdateMoveSpeed(int _speed);			
	void VerticalTurn();
};

#endif BIRD_H