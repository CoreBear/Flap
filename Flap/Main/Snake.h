#ifndef BIRD_H
#define BIRD_H

#include "DList.h"
#include "Enums.h"
#include "SceneObject.h"

namespace Structure { struct CollisionRenderInfo; };
namespace Structure { struct Generic; };
class SharedGame;
namespace Structure { template<class Type> struct Vector2; }

class Snake : public SceneObject
{
public:
	// Static Variables
	static Structure::Generic s_genericContainer;

	// Initialization
	virtual void Initialize(const Structure::Generic* const _genericContainer) override;
	inline Snake() :
		m_currentDirection(Enums::InputName::NA),
		m_newDirection(Enums::InputName::NA),
		m_numberOfTailSectionsToAdd(Consts::NO_VALUE)
	{
		m_newCollisionRenderInfo.m_char = Consts::EMPTY_SPACE_CHAR;
	}
	Snake(const Snake&) = delete;
	Snake& operator=(const Snake&) = delete;

	// Updates
	void FixedUpdate() override;

	// Functionality
	bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) override final;
	void Pause();
	void Resume();

	// Destruction
	void Destroy(bool _cleanScene) override final;

protected:
	// Member Variables
	Structure::CollisionRenderInfo m_newCollisionRenderInfo;

	// Functionality
	inline void InputDown() { m_newDirection = Enums::InputName::Down; }
	inline void InputLeft() { m_newDirection = Enums::InputName::Left; }
	inline void InputRight() { m_newDirection = Enums::InputName::Right; }
	inline void InputUp() { m_newDirection = Enums::InputName::Up; }

private:
	// Static Variables
	static int s_snakeStartingSpeed;

	// Member Variables
	DList<Structure::CollisionRenderInfo> m_bodyNodes;
	DList<Structure::CollisionRenderInfo>::Iterator m_headTraversingIterator;
	DList<Structure::CollisionRenderInfo>::Iterator m_tailTraversingIterator;
	Enums::InputName m_currentDirection;
	Enums::InputName m_newDirection;
	int m_currentSpeed;
	int m_length;
	unsigned int m_moveTargetFrame;
	unsigned int m_numberOfFramesBetweenMoves;
	unsigned int m_numberOfFramesLeftBeforePause;
	unsigned int m_numberOfTailSectionsToAdd;
	Structure::Vector2<int> m_newTailPosition;

	// Functionality
	void Death();
	void Move();
	void TryAddTail();
	void TryTurn();
	void WriteIntoFrameBuffer();
};

#endif BIRD_H