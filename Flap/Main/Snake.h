#ifndef BIRD_H
#define BIRD_H

#include "Enums.h"
#include "InputReceiver.h"
#include "SceneObject.h"
#include "Structure.h"

#include <list>

class SharedMemory;

class Snake : public SceneObject
{
public:
	// Static Variables
	static SharedMemory* sp_sharedMemory;

	// Static Initialization
	inline static void AssignSharedMemory(SharedMemory& _sharedMemory) { sp_sharedMemory = &_sharedMemory; }

	// Initialization
	void Initialize(const Structure::Generic& _genericContainer) override final;
	Snake();
	Snake(const Snake&) = delete;
	Snake& operator=(const Snake&) = delete;

	// Updates
	void FixedUpdate() override;

	// Functionality
	void Collision(const SceneObject& _otherCollidingObject, const Structure::Vector2& _collisionCellCR) override final;

protected:
	// Member Variables

	// Functionality
	inline void InputDown() { m_newDirection = Enums::Direction::Down; }
	inline void InputLeft() { m_newDirection = Enums::Direction::Left; }
	inline void InputRight() { m_newDirection = Enums::Direction::Right; }
	inline void InputUp() { m_newDirection = Enums::Direction::Up; }

private:
	// Member Variables
	Enums::Direction m_currentDirection;
	Enums::Direction m_newDirection;
	const Structure::Generic* OTHER_COLLISION_PACKAGE;
	std::list<Structure::Vector2> m_listOfBodyPositions;
	std::list<Structure::Vector2>::iterator m_headTraversingIterator;
	std::list<Structure::Vector2>::iterator m_tailTraversingIterator;
	std::list<Structure::Vector2>::iterator m_tailIterator;
	unsigned int m_moveTargetFrame;
	unsigned int m_numberOfFramesPerCell;
	unsigned int m_numberOfFramesPerCellHorizontal;
	unsigned int m_numberOfFramesPerCellVertical;
	unsigned int m_numberOfTailSectionsToAdd;
	Structure::Vector2 m_newTailPosition;

	// Functionality
	void Death();
	void HorizontalTurn();
	void Move();
	void TryAddTail();
	void TryTurn();
	void Turn();
	void UpdateMoveSpeed(int _speed);			
	void VerticalTurn();
};

#endif BIRD_H