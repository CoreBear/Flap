#ifndef BUFFER_WRITER
#define BUFFER_WRITER

#include "DList.h"
#include "Manager.h"
#include "Structure.h"

#include <mutex>

class SharedMemory;
class SceneObject;

class BufferManager final : public Manager
{
public:
	// Initialization
	BufferManager(SharedMemory& _sharedMemory);
	BufferManager(const BufferManager&) = delete;
	BufferManager& operator=(const BufferManager&) = delete;

	// Updates
	void Update() override;

private:
	// Member Variables
	bool m_firstWriteInForObject;
	bool m_frameWritingIsComplete;
	bool m_writeSpritesIntoBuffer;
	Structure::BufferCell* mp_bufferCell;
	DList<Structure::Vector2>::Const_Iterator m_positionIterator;
	const DList<SceneObject*>::Const_Iterator& NULL_ITERATOR;
	SharedMemory& mr_sharedMemory;
	const short FOOD_COLOR;
	const short SNAKE_BODY_COLOR;
	const short SNAKE_HEAD_COLOR;
	const Structure::SnakeRenderInfo* SNAKE_COLLISION_RENDER_INFO;
	std::unique_lock<std::mutex> m_bufferWriterIteratorUniqueLock;

	// Functionality
	void SetSnakeWriteAttributes(const Structure::RenderInfo& _renderInfo);
	void WriteIntoBuffer(const Structure::RenderInfo& _renderInfo);
	void WriteIntoBufferCell(const Structure::RenderInfo& _renderInfo);
};

#endif BUFFER_WRITER