#ifndef BIRD_H
#define BIRD_H

#include "DList.h"
#include "Enums.h"
#include "SceneObject.h"

class BufferCell;
namespace Structure { struct CollisionRenderInfo; };
namespace Structure { struct Generic; };
class SharedGame;
namespace Structure { struct Vector2; };

class Snake : public SceneObject
{
public:
	// Static Variables
	static bool s_moveThisFrame;
	static Structure::Generic s_genericContainer;
	static unsigned int s_numberOfFramesBetweenMoves;
	static unsigned int s_numberOfFramesLeftBeforePause;
	static unsigned int s_snakeMoveTargetFrame;

	// Static Initialization
	static void AssignSharedGame(SharedGame& _sharedGame) { sp_sharedGame = &_sharedGame; }

	// Initialization
	void Initialize(const Structure::Generic* const _genericContainer) override final;
	Snake();
	Snake(const Snake&) = delete;
	Snake& operator=(const Snake&) = delete;

	// Updates
	void FixedUpdate() override;

	// Functionality
	bool Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf = false) override final;

	// Destruction
	void Denitialize(bool _removeFromSceneObjects) override final;

protected:
	// Functionality
	inline void InputDown() { m_newDirection = Enums::InputName::Down; }
	inline void InputLeft() { m_newDirection = Enums::InputName::Left; }
	inline void InputRight() { m_newDirection = Enums::InputName::Right; }
	inline void InputUp() { m_newDirection = Enums::InputName::Up; }

private:
	// Static Variables
	static SharedGame* sp_sharedGame;

	// Member Variables
	BufferCell* mp_bufferCell;
	Structure::CollisionRenderInfo m_newCollisionRenderInfo;
	DList<Structure::CollisionRenderInfo> m_bodyNodes;
	DList<Structure::CollisionRenderInfo>::Iterator m_headTraversingIterator;
	DList<Structure::CollisionRenderInfo>::Iterator m_tailTraversingIterator;
	Enums::InputName m_currentDirection;
	Enums::InputName m_newDirection;
	unsigned int m_numberOfTailSectionsToAdd;
	Structure::Vector2 m_newTailPosition;

	// Functionality
	void Death();
	void Move();
	void TryAddTail();
	void TryTurn();
	void WriteIntoFrameBuffer();
};

#endif BIRD_H