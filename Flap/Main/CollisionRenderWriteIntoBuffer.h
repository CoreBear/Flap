#ifndef COLLISION_RENDER_WRITE_INTO_BUFFER_H
#define COLLISION_RENDER_WRITE_INTO_BUFFER_H

#include "DList.h"
#include "GameThreadBase.h"
#include "Structure.h"

#include <mutex>

class BufferCell;
class MenuBase;
class SceneObject;
class SharedCollisionRender;
class SharedGame;

class CollisionRenderWriteIntoBuffer final : public GameThreadBase
{
public:
	// Initialization
	CollisionRenderWriteIntoBuffer(SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame);
	CollisionRenderWriteIntoBuffer(const CollisionRenderWriteIntoBuffer&) = delete;
	CollisionRenderWriteIntoBuffer& operator=(const CollisionRenderWriteIntoBuffer&) = delete;

	// Updates
	void Update() override;

private:
	// Member Variables
	bool m_firstWriteInForObject;
	bool m_frameWritingIsComplete;
	bool m_writeSpritesIntoBuffer;
	BufferCell* mp_bufferCell;
	const char* mp_walker;
	DList<Structure::Vector2>::Const_Iterator m_positionIterator;
	const DList<SceneObject*>::Const_Iterator& NULL_ITERATOR;
	int m_reusableIterator;
	int m_textLetterColumnPosition;
	MenuBase** mpp_menus;
	SharedCollisionRender& mr_sharedCollisionRender;
	SharedGame& mr_sharedGame;
	short m_lineColor;
	const short FOOD_COLOR;
	const short SNAKE_BODY_COLOR;
	const short SNAKE_HEAD_COLOR;
	const Structure::SnakeRenderInfo* SNAKE_COLLISION_RENDER_INFO;
	std::unique_lock<std::mutex> m_bufferWriterIteratorUniqueLock;
	std::unique_lock<std::mutex> m_menuUniqueLock;

	// Functionality
	void GameUpdate();
	void MenuUpdate();
	void SetSnakeWriteAttributes(const Structure::RenderInfo& _renderInfo);
	void WriteIntoBuffer();
	void WriteIntoBuffer(const Structure::RenderInfo& _renderInfo);
	void WriteIntoBufferCell(const Structure::RenderInfo& _renderInfo);
	void WriteTextLineIntoBuffer(bool _highlightLine, const Structure::TextLine& _textLine);

};

#endif COLLISION_RENDER_WRITE_INTO_BUFFER_H