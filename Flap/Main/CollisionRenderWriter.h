#ifndef COLLISION_RENDER_WRITER_H
#define COLLISION_RENDER_WRITER_H

#include "DList.h"
#include "GameThreadBase.h"
#include "Structure.h"

#include <mutex>

class BufferCell;
class SharedCollisionRender;
class SceneObject;

class CollisionRenderWriter final : public GameThreadBase
{
public:
	// Initialization
	CollisionRenderWriter(SharedCollisionRender& _sharedCollisionRender);
	CollisionRenderWriter(const CollisionRenderWriter&) = delete;
	CollisionRenderWriter& operator=(const CollisionRenderWriter&) = delete;

	// Updates
	void Update() override;

private:
	// Member Variables
	bool m_firstWriteInForObject;
	bool m_frameWritingIsComplete;
	bool m_writeSpritesIntoBuffer;
	BufferCell* mp_bufferCell;
	DList<Structure::Vector2>::Const_Iterator m_positionIterator;
	const DList<SceneObject*>::Const_Iterator& NULL_ITERATOR;
	SharedCollisionRender& mr_sharedCollisionRender;
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

#endif COLLISION_RENDER_WRITER_H