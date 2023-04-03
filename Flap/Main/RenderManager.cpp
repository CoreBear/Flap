#pragma region Includes
#include "RenderManager.h"

#include "Consts.H"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedMemory.h"

#include <iterator>
#include <Windows.h>
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory) :
	m_frameWritingIsComplete(false),
	m_writeSpritesIntoBuffer(false),
	mp_bufferCell(nullptr),
	m_bufferSize(_sharedMemory.SCREEN_BUFFER_CR.X* _sharedMemory.SCREEN_BUFFER_CR.Y),
	mp_frameBuffer(new BufferCell[m_bufferSize]),
	mp_textBuffer(new CHAR_INFO[m_bufferSize]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator(Consts::NO_VALUE),
	NULL_ITERATOR(DList<SceneObject*>::Const_Iterator()),
	mp_sharedMemory(&_sharedMemory),
	FOOD_COLOR(static_cast<short>(BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)),
	SNAKE_BODY_COLOR(static_cast<short>(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)),
	SNAKE_HEAD_COLOR(static_cast<short>(BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)),
	SNAKE_COLLISION_RENDER_INFO(nullptr),
	m_renderIteratorUniqueLock(_sharedMemory.GetRenderIteratorMutexRef())
{

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	{
		mp_frameBuffer[m_reusableIterator].ResetCell(NULL_ITERATOR);
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);

	m_writeRegionRect.Bottom = static_cast<SHORT>(_sharedMemory.SCREEN_BUFFER_CR.Y - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(_sharedMemory.SCREEN_BUFFER_CR.X - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_renderIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	m_renderIteratorUniqueLock.lock();

	// If Scene thread is already waiting
	if (mp_sharedMemory->m_threadWaitingFlag)
	{
		// Let it know it can release
		mp_sharedMemory->m_renderIteratorConVar.notify_one();

		// Then wait for Scene thread to let this (Render) thread know it can release, because Scene thread needs to go first
		mp_sharedMemory->m_renderIteratorConVar.wait(m_renderIteratorUniqueLock);
	}

	// If Scene thread is not already waiting
	else
	{
		// Notify system that this (Render) thread is waiting
		mp_sharedMemory->m_threadWaitingFlag = true;

		// When Scene thread releases this (Render) thread
		mp_sharedMemory->m_renderIteratorConVar.wait(m_renderIteratorUniqueLock);
	}

	// Notify system that this (Render) thread is no longer waiting
	// NOTE: Scene thread will always be released first, so this is not needed on that side
	mp_sharedMemory->m_threadWaitingFlag = false;

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_renderIteratorUniqueLock.unlock();

	m_frameWritingIsComplete = false;

	// Write next frame
	while (m_frameWritingIsComplete == false)
	{
		// Check to see if there are scene objects to write in
		m_renderIteratorUniqueLock.lock();
		m_writeSpritesIntoBuffer = mp_sharedMemory->m_renderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
		m_renderIteratorUniqueLock.unlock();

		// While there are scene objects to write
		while (m_writeSpritesIntoBuffer)
		{
			// Write scene object into buffer
			WriteIntoBuffer((*mp_sharedMemory->m_renderIterator)->GetRenderInfoRef());

			// Move to next scene object
			++mp_sharedMemory->m_renderIterator;

			// Check to see if there are scene objects to write in
			m_renderIteratorUniqueLock.lock();
			m_writeSpritesIntoBuffer = mp_sharedMemory->m_renderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
			m_renderIteratorUniqueLock.unlock();
		}

		// Check to see if all scene objects have been written in (frame write is complete)
		m_renderIteratorUniqueLock.lock();
		m_frameWritingIsComplete = mp_sharedMemory->m_renderIterator == mp_sharedMemory->NULL_ITERATOR;
		m_renderIteratorUniqueLock.unlock();
	}

	// Check collision(s) and render frame (generated above)
	{
		// For each cell
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
		{
			if (mp_frameBuffer[m_reusableIterator].m_state == BufferCell::State::Collision)
			{
				// Get column and row
				m_collisionCellCR.m_x = m_reusableIterator % mp_sharedMemory->SCREEN_BUFFER_CR.X;
				m_collisionCellCR.m_y = m_reusableIterator / mp_sharedMemory->SCREEN_BUFFER_CR.X;

				// Send each object, the other object and the collision cell's column and row
				(*mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE])->Collision(*(*mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE]), m_collisionCellCR);
				(*mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE])->Collision(*(*mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE]), m_collisionCellCR);
			}

			mp_textBuffer[m_reusableIterator].Attributes = mp_frameBuffer[m_reusableIterator].m_colorBFGround;
			mp_textBuffer[m_reusableIterator].Char.UnicodeChar = mp_frameBuffer[m_reusableIterator].m_character;
		}

		// Render from buffer
		WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_textBuffer, mp_sharedMemory->SCREEN_BUFFER_CR, m_topLeftCellCR, &m_writeRegionRect);
	}

	// Reset the buffer
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	{
		mp_frameBuffer[m_reusableIterator].ResetCell(NULL_ITERATOR);
	}
}
#pragma endregion

#pragma region Private Functionality
void RenderManager::SetSnakeWriteAttributes(const Structure::RenderInfo& _renderInfo)
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
void RenderManager::WriteIntoBuffer(const Structure::RenderInfo& _renderInfo)
{
	if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Food)
	{
		// Store the memory address of the cell that's being updated
		mp_bufferCell = &mp_frameBuffer[(_renderInfo.POSITION.m_y * mp_sharedMemory->SCREEN_BUFFER_CR.X) + _renderInfo.POSITION.m_x];

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
			mp_bufferCell = &mp_frameBuffer[(m_positionIterator->m_y * mp_sharedMemory->SCREEN_BUFFER_CR.X) + m_positionIterator->m_x];

			// Write into buffer
			WriteIntoBufferCell(_renderInfo);
		}
	}
}
void RenderManager::WriteIntoBufferCell(const Structure::RenderInfo& _renderInfo)
{
	// NOTE: Notice the increment
	mp_bufferCell->mp_objectsInCellIterators[mp_bufferCell->m_objectInCellIndex++] = mp_sharedMemory->m_renderIterator;

	// What is currently in the cell
	switch (mp_bufferCell->m_state)
	{
		// Do nothing, shouldn't happen
	//case BufferCell::State::Collision:

	case BufferCell::State::Empty:
	{
		if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Food)
		{
			mp_bufferCell->m_character = _renderInfo.m_character;
			mp_bufferCell->m_colorBFGround = FOOD_COLOR;
			mp_bufferCell->m_state = BufferCell::State::Food;
		}

		// If snake
		else
		{
			SetSnakeWriteAttributes(_renderInfo);

			mp_bufferCell->m_state = BufferCell::State::Snake;
		}
	}
	break;

	// NOTE: Only the snake matters in this collision, because the food will disappear
	case BufferCell::State::Food:
	case BufferCell::State::Snake:
	{
		if (_renderInfo.OBJECT_TYPE == Enums::ObjectType::Snake)
		{
			SetSnakeWriteAttributes(_renderInfo);
		}

		mp_bufferCell->m_state = BufferCell::State::Collision;
	}
	break;
	}
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete[] mp_frameBuffer;
	delete[] mp_textBuffer;
	delete mp_sharedMemory;
}
#pragma endregion