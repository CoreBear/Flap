#pragma region Includes
#include "CollisionRenderWriteIntoBuffer.h"

#include "BufferCell.h"
#include "Consts.h"
#include "MenuBase.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedCollisionRender.h"
#include "SharedGame.h"
#pragma endregion

#pragma region Initialization
CollisionRenderWriteIntoBuffer::CollisionRenderWriteIntoBuffer(SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame) :
	m_firstWriteInForObject(false),
	m_frameWritingIsComplete(false),
	m_writeSpritesIntoBuffer(false),
	mp_bufferCell(nullptr),
	NULL_ITERATOR(DList<SceneObject*>::Const_Iterator()),
	mr_sharedCollisionRender(_sharedCollisionRender),
	mr_sharedGame(_sharedGame),
	FOOD_COLOR(static_cast<short>(BACKGROUND_BLUE | Consts::WHITE_FOREGROUND)),
	SNAKE_BODY_COLOR(static_cast<short>(Consts::WHITE_BACKGROUND)),
	SNAKE_HEAD_COLOR(static_cast<short>(BACKGROUND_RED | Consts::WHITE_FOREGROUND)),
	SNAKE_COLLISION_RENDER_INFO(nullptr),
	m_bufferWriterIteratorUniqueLock(_sharedCollisionRender.m_bufferWriterIteratorMutex),
	m_menuUniqueLock(_sharedCollisionRender.m_menuMutex)
{
	WriteIntoBuffer();

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_bufferWriterIteratorUniqueLock.unlock();
	m_menuUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void CollisionRenderWriteIntoBuffer::Update()
{
	mr_sharedGame.m_gameStateMutex.lock();

	switch (mr_sharedGame.m_gameState)
	{
	case Enums::GameState::Game:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		GameUpdate();
	}
	break;
	case Enums::GameState::Menu:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		MenuUpdate();
	}
	break;
	default:
	{
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	}
}
#pragma endregion

#pragma region Private Functionality
void CollisionRenderWriteIntoBuffer::GameUpdate()
{
	m_bufferWriterIteratorUniqueLock.lock();

	// If Scene thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();

		// Then wait for Scene thread to let this (Buffer) thread know it can release, because Scene thread needs to go first
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.wait(m_bufferWriterIteratorUniqueLock);

		// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Scene thread is not already waiting
	else
	{
		// Notify system that this (Buffer) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Scene thread releases this (Buffer) thread
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.wait(m_bufferWriterIteratorUniqueLock);
	}

	mr_sharedGame.m_gameStateMutex.lock();
	switch (mr_sharedGame.m_gameState)
	{
	case Enums::GameState::Menu:
	case Enums::GameState::ExitToResults:
	{
		mr_sharedCollisionRender.m_threadWaitingFlag = false;

		mr_sharedGame.m_gameStateMutex.unlock();
		m_bufferWriterIteratorUniqueLock.unlock();

		WriteIntoBuffer();
	}
	return;
	}
	mr_sharedGame.m_gameStateMutex.unlock();
	m_bufferWriterIteratorUniqueLock.unlock();

	m_frameWritingIsComplete = false;

	// Write next frame
	while (m_frameWritingIsComplete == false)
	{
		// Check to see if there are scene objects to write in
		m_writeSpritesIntoBuffer = mr_sharedCollisionRender.m_bufferWriterIterator != mr_sharedCollisionRender.m_sceneObjectsIterator;

		// While there are scene objects to write
		while (m_writeSpritesIntoBuffer)
		{
			// Write scene object into buffer
			WriteIntoBuffer((*mr_sharedCollisionRender.m_bufferWriterIterator)->GetRenderInfoRef());

			// Move to next scene object
			++mr_sharedCollisionRender.m_bufferWriterIterator;

			// Check to see if there are scene objects to write in
			m_writeSpritesIntoBuffer = mr_sharedCollisionRender.m_bufferWriterIterator != mr_sharedCollisionRender.m_sceneObjectsIterator;
		}

		// Check to see if all scene objects have been written in (frame write is complete)
		m_frameWritingIsComplete = mr_sharedCollisionRender.m_bufferWriterIterator == mr_sharedCollisionRender.NULL_ITERATOR;
	}

	mr_sharedCollisionRender.m_renderMutex.lock();
	mr_sharedCollisionRender.m_somethingToRender = true;
	mr_sharedCollisionRender.m_renderMutex.unlock();

	m_bufferWriterIteratorUniqueLock.lock();

	// If Scene thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();

		// Notify the system that nothing is waiting (when this is unlocked and the Scene thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Scene thread is not already waiting
	else
	{
		// Notify system that this (Buffer) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Render thread releases this (Buffer) thread, flip flag
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.wait(m_bufferWriterIteratorUniqueLock);
	}

	m_bufferWriterIteratorUniqueLock.unlock();
}
void CollisionRenderWriteIntoBuffer::MenuUpdate()
{
	m_menuUniqueLock.lock();

	// If Scene thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_menuConVar.notify_one();

		// Then wait for Scene thread to let this (Buffer) thread know it can release, because Scene thread needs to go first
		mr_sharedCollisionRender.m_menuConVar.wait(m_menuUniqueLock);

		// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Scene thread is not already waiting
	else
	{
		// Notify system that this (Buffer) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Scene thread releases this (Buffer) thread
		mr_sharedCollisionRender.m_menuConVar.wait(m_menuUniqueLock);
	}

	mr_sharedGame.m_gameStateMutex.lock();
	switch (mr_sharedGame.m_gameState)
	{
	case Enums::GameState::ExitApp:
	{
		mr_sharedGame.m_gameStateMutex.unlock();
		m_menuUniqueLock.unlock();
	}
	return;
	case Enums::GameState::Game:
	{
		mr_sharedCollisionRender.m_threadWaitingFlag = false;

		mr_sharedGame.m_gameStateMutex.unlock();
		m_menuUniqueLock.unlock();
	}
	return;
	}
	mr_sharedGame.m_gameStateMutex.unlock();
	m_menuUniqueLock.unlock();

	WriteIntoBuffer();

	m_menuUniqueLock.lock();

	// If Scene thread is already waiting
	if (mr_sharedCollisionRender.m_threadWaitingFlag)
	{
		// Release it
		mr_sharedCollisionRender.m_menuConVar.notify_one();

		// Notify the system that nothing is waiting (when this is unlocked and the Scene thread grabs the lock)
		mr_sharedCollisionRender.m_threadWaitingFlag = false;
	}

	// If Scene thread is not already waiting
	else
	{
		// Notify system that this (Buffer) thread is waiting
		mr_sharedCollisionRender.m_threadWaitingFlag = true;

		// When Render thread releases this (Buffer) thread, flip flag
		mr_sharedCollisionRender.m_menuConVar.wait(m_menuUniqueLock);
	}

	m_menuUniqueLock.unlock();
}
void CollisionRenderWriteIntoBuffer::SetSnakeWriteAttributes(const Structure::RenderInfo& _renderInfo)
{
	// If snake head, flip flag and write player number
	if (m_firstWriteInForObject)
	{
		m_firstWriteInForObject = false;

		mp_bufferCell->m_character = _renderInfo.m_character;
		mp_bufferCell->m_colorBFGround = SNAKE_HEAD_COLOR;
	}

	// If snake body
	else
	{

		mp_bufferCell->m_character = Consts::EMPTY_SPACE_CHAR;
		mp_bufferCell->m_colorBFGround = SNAKE_BODY_COLOR;
	}
}
void CollisionRenderWriteIntoBuffer::WriteIntoBuffer()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedCollisionRender.mp_menu->m_numberOfTextLines; m_reusableIterator++)
	{
		WriteTextLineIntoBuffer(mr_sharedCollisionRender.mp_menu->GetCurrentButtonNumber() == m_reusableIterator, *mr_sharedCollisionRender.mp_menu->mp_textLines[m_reusableIterator]);
	}

	mr_sharedCollisionRender.m_renderMutex.lock();
	mr_sharedCollisionRender.m_somethingToRender = true;
	mr_sharedCollisionRender.m_renderMutex.unlock();
}
void CollisionRenderWriteIntoBuffer::WriteIntoBuffer(const Structure::RenderInfo& _renderInfo)
{
	if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Food)
	{
		// Store the memory address of the cell that's being updated
		mp_bufferCell = &mr_sharedCollisionRender.mp_frameBuffer[(_renderInfo.POSITION.m_y * mr_sharedCollisionRender.SCREEN_BUFFER_CR.X) + _renderInfo.POSITION.m_x];

		// Write into buffer
		WriteIntoBufferCell(_renderInfo);
	}

	// If snake
	else
	{
		m_firstWriteInForObject = true;

		SNAKE_COLLISION_RENDER_INFO = dynamic_cast<const Structure::SnakeRenderInfo*>(&_renderInfo);

		// For each node
		for (m_positionIterator = SNAKE_COLLISION_RENDER_INFO->LIST_OF_BODY_NODES.Begin(); m_positionIterator != SNAKE_COLLISION_RENDER_INFO->LIST_OF_BODY_NODES.End(); ++m_positionIterator)
		{
			// Store the memory address of the cell that's being updated
			mp_bufferCell = &mr_sharedCollisionRender.mp_frameBuffer[(m_positionIterator->m_y * mr_sharedCollisionRender.SCREEN_BUFFER_CR.X) + m_positionIterator->m_x];

			// Write into buffer
			WriteIntoBufferCell(_renderInfo);
		}
	}
}
void CollisionRenderWriteIntoBuffer::WriteIntoBufferCell(const Structure::RenderInfo& _renderInfo)
{
	// NOTE: Notice the increment
	mp_bufferCell->mp_objectsInCellIterators[mp_bufferCell->m_objectInCellIndex++] = mr_sharedCollisionRender.m_bufferWriterIterator;

	// What is currently in the cell
	switch (mp_bufferCell->m_cellState)
	{
		// Do nothing, shouldn't happen
	//case Enums::CellState::Collision:

	case Enums::CellState::Empty:
	{
		if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Food)
		{
			mp_bufferCell->m_cellState = Enums::CellState::Food;
			mp_bufferCell->m_character = _renderInfo.m_character;
			mp_bufferCell->m_colorBFGround = FOOD_COLOR;
		}

		// If snake
		else
		{
			SetSnakeWriteAttributes(_renderInfo);

			mp_bufferCell->m_cellState = Enums::CellState::Snake;
		}
	}
	break;

	// NOTE: Only the snake matters in this collision, because the food will disappear
	case Enums::CellState::Food:
	case Enums::CellState::Snake:
	{
		if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Snake)
		{
			SetSnakeWriteAttributes(_renderInfo);
		}

		mp_bufferCell->m_cellState = Enums::CellState::Collision;
	}
	break;
	}
}
void CollisionRenderWriteIntoBuffer::WriteTextLineIntoBuffer(bool _highlightLine, const Structure::TextLine& _textLine)
{
	// NOTE: I know I can use strlen, but I wanted to do it this way. I'll use strlen in Tools

	mp_walker = _textLine.m_text;
	m_textLetterColumnPosition = _textLine.m_position.m_x;

	// If option should be highlighted, else not
	m_lineColor = (_highlightLine) ? Consts::WHITE_BACKGROUND : Consts::WHITE_FOREGROUND;

	while (*mp_walker != '\0')
	{
		mp_bufferCell = &mr_sharedCollisionRender.mp_frameBuffer[(_textLine.m_position.m_y * mr_sharedCollisionRender.SCREEN_BUFFER_CR.X) + m_textLetterColumnPosition++];
		mp_bufferCell->m_character = *mp_walker;
		mp_bufferCell->m_colorBFGround = m_lineColor;

		++mp_walker;
	}
}
#pragma endregion