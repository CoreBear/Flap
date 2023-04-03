#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "DList.h"
#include "Manager.h"
#include "Structure.h"

#include <mutex>
#include <Windows.h>

class SharedMemory;
class SceneObject;

class CollisionRenderManager final : public Manager
{
public:
	// Initialization
	CollisionRenderManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory);
	CollisionRenderManager(const CollisionRenderManager&) = delete;
	CollisionRenderManager& operator=(const CollisionRenderManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~CollisionRenderManager() override;

private:
	// Container
	struct BufferCell
	{
	public:
		// Static Variables
		static constexpr int MAX_NUMBER_OF_OBJECTS_IN_CELL = 2;
		enum class State { Collision, Empty, Food, Snake };

		// Member Variables
		char m_character;
		int m_objectInCellIndex;
		DList<SceneObject*>::Const_Iterator mp_objectsInCellIterators[MAX_NUMBER_OF_OBJECTS_IN_CELL];
		short m_colorBFGround;
		State m_state;

		// Functionality
		void ResetCell(const DList<SceneObject*>::Const_Iterator _nullIterator)
		{
			m_character = Consts::EMPTY_SPACE_CHAR;
			m_state = State::Empty;
			m_objectInCellIndex = Consts::NO_VALUE;
			mp_objectsInCellIterators[Consts::NO_VALUE] = _nullIterator;
			mp_objectsInCellIterators[Consts::OFF_BY_ONE] = _nullIterator;
			m_colorBFGround = NULL;
		}
	};

	// Member Variables
	bool m_frameWritingIsComplete;
	bool m_firstWriteInForObject;
	bool m_writeSpritesIntoBuffer;
	BufferCell* mp_bufferCell;
	int m_bufferSize;							// NOTE/WARNING: This needs to be here, because it needs to initialize before the buffers
	BufferCell* const mp_frameBuffer;
	CHAR_INFO* const mp_textBuffer;
	COORD m_topLeftCellCR;
	const HANDLE& OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;
	DList<Structure::Vector2>::Const_Iterator m_positionIterator;
	const DList<SceneObject*>::Const_Iterator& NULL_ITERATOR;
	SharedMemory* const mp_sharedMemory;
	const short FOOD_COLOR;
	const short SNAKE_BODY_COLOR;
	const short SNAKE_HEAD_COLOR;
	SMALL_RECT m_writeRegionRect;
	const Structure::SnakeCollisionRenderInfo* SNAKE_COLLISION_RENDER_INFO;
	std::unique_lock<std::mutex> m_collisionRenderIteratorUniqueLock;
	Structure::Vector2 m_collisionCellCR;

	// Functionality
	void SetSnakeWriteAttributes(const Structure::CollisionRenderInfo& _collisionRenderInfo);
	void WriteIntoBuffer(const Structure::CollisionRenderInfo& _collisionRenderInfo);
	void WriteIntoBufferCell(const Structure::CollisionRenderInfo& _collisionRenderInfo);
};

#endif RENDER_MANAGER_H