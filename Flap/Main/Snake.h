#ifndef BIRD_H
#define BIRD_H

#include "DList.h"
#include "Enums.h"
#include "SceneObject.h"

class SharedCollisionRender;

namespace Structure { struct Vector2; };

class Snake : public SceneObject
{
public:
	// Static Variables
	static Structure::Generic s_genericContainer;

	// Static Initialization
	inline static void AssignSharedCollisionRender(SharedCollisionRender& _sharedCollisionRender) { sp_sharedCollisionRender = &_sharedCollisionRender; }

	// Initialization
	void Initialize(const Structure::Generic* const _genericContainer) override final;
	Snake();
	Snake(const Snake&) = delete;
	Snake& operator=(const Snake&) = delete;

	// Updates
	void FixedUpdate() override;

	// Functionality
	void Collision(const Structure::Generic* const _otherCollisionPackage, const Structure::Vector2& _collisionCellCR) override final;

protected:
	// Functionality
	inline void InputDown() { m_newDirection = Enums::Direction::Down; }
	inline void InputLeft() { m_newDirection = Enums::Direction::Left; }
	inline void InputRight() { m_newDirection = Enums::Direction::Right; }
	inline void InputUp() { m_newDirection = Enums::Direction::Up; }

private:
	// Static Variables
	static SharedCollisionRender* sp_sharedCollisionRender;

	// Member Variables
	DList<Structure::Vector2> m_bodyNodes;
	DList<Structure::Vector2>::Iterator m_headTraversingIterator;
	DList<Structure::Vector2>::Iterator m_tailTraversingIterator;
	Enums::Direction m_currentDirection;
	Enums::Direction m_newDirection;
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