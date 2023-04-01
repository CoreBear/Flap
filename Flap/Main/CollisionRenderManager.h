#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Manager.h"
#include "Structure.h"

#include <list>
#include <mutex>
#include <Windows.h>

class SharedMemory;
class SceneObject;

class CollisionRenderManager final : public Manager
{
public:
	// Initialization
	CollisionRenderManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory, const Structure::Vector2& _bufferSize);

	// Updates
	void Update() override;

	// Destruction
	~CollisionRenderManager() override;

private:
	// Container
	struct BufferCell
	{
	public:
		// Member Variables
		enum class State { Collision, Empty, Food, Snake };
		int m_objectInCellIndex;
		static constexpr int MAX_NUMBER_OF_OBJECTS_IN_CELL = 2;
		std::list<SceneObject*>::const_iterator m_objectsInCellIterators[MAX_NUMBER_OF_OBJECTS_IN_CELL];
		State m_state;

		// Functionality
		void ResetCell(const std::list<SceneObject*>::const_iterator& _nullIterator)
		{
			m_objectInCellIndex = Consts::NO_VALUE;
			m_objectsInCellIterators[0] = _nullIterator;
			m_objectsInCellIterators[1] = _nullIterator;
			m_state = State::Empty;
		}
	};

	// Member Variables
	bool m_frameWritingIsComplete;
	bool m_writeSpritesIntoBuffer;
	BufferCell* mp_bufferCell;
	BufferCell* mp_bufferForReading;
	BufferCell* mp_bufferForWriting;
	BufferCell* mp_bufferSwapper;
	CHAR_INFO* mp_textBuffer;
	COORD m_screenBufferCR;
	COORD m_topLeftCellCR;
	const HANDLE& mr_outputWindowHandle;
	int m_bufferSize;
	int m_reusableIterator;
	const std::list<SceneObject*>::const_iterator& mr_nullIterator;
	std::list<Structure::Vector2>::const_iterator m_positionIterator;
	SharedMemory* mp_sharedMemory;
	short m_colorWhite;
	SMALL_RECT m_writeRegionRect;
	const Structure::SnakeCollisionRenderInfo* m_snakeCollisionRenderInfo;
	std::unique_lock<std::mutex> m_collisionRenderIteratorUniqueLock;

	// Functionality
	void WriteIntoBuffer(const Structure::CollisionRenderInfo& _collisionRenderInfo);
	void WriteIntoBufferCell(const Structure::CollisionRenderInfo& _collisionRenderInfo);
};

#endif RENDER_MANAGER_H