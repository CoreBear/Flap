#pragma region Includes
#include "CollisionRenderManager.h"

#include "Consts.H"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedMemory.h"

#include <iterator>
#include <Windows.h>
#pragma endregion

#pragma region Initialization
CollisionRenderManager::CollisionRenderManager(HANDLE& _windowHandle, SharedMemory& _sharedMemory, const Structure::Vector2<int>& _bufferSize) :
	m_frameWritingIsComplete(false),
	m_writeSpritesIntoBuffer(false),
	mp_bufferCell(nullptr),
	mp_bufferSwapper(nullptr),
	mp_windowHandle(&_windowHandle), 
	m_bufferSize(_bufferSize.m_x * _bufferSize.m_y),
	m_reusableIterator(Consts::NO_VALUE), 
	mr_nullIterator(_sharedMemory.GetNullIteratorRef()),
	mp_sharedMemory(&_sharedMemory),
	m_collisionRenderIteratorUniqueLock(_sharedMemory.GetCollisionRenderIteratorMutexRef())
{
	int numberOfWindowColumns = _bufferSize.m_x;
	int numberOfWindowRows = _bufferSize.m_y;

	mp_bufferForReading = new BufferCell[m_bufferSize];
	mp_bufferForWriting = new BufferCell[m_bufferSize];
	mp_textBuffer = new CHAR_INFO[m_bufferSize];

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	{
		mp_bufferForReading[m_reusableIterator].ResetCell(mr_nullIterator);
		mp_bufferForWriting[m_reusableIterator].ResetCell(mr_nullIterator);
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_screenBufferCR.X = numberOfWindowColumns;
	m_screenBufferCR.Y = numberOfWindowRows;
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);

	m_colorWhite = static_cast<short>(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

	m_writeRegionRect.Bottom = static_cast<SHORT>(numberOfWindowRows - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(numberOfWindowColumns - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_collisionRenderIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void CollisionRenderManager::Update()
{
	m_collisionRenderIteratorUniqueLock.lock();

	// If Scene thread is already waiting
	if (mp_sharedMemory->m_threadWaitingFlag)
	{
		// Let it know it can release
		mp_sharedMemory->m_collisionRenderIteratorConVar.notify_one();

		// Then wait for Scene thread to let this (CollisionRender) thread know it can release, because Scene thread needs to go first
		mp_sharedMemory->m_collisionRenderIteratorConVar.wait(m_collisionRenderIteratorUniqueLock);
	}

	// If Scene thread is not already waiting, flip flag and wait
	else
	{
		mp_sharedMemory->m_threadWaitingFlag = true;

		// When Scene thread releases this (CollisionRender) thread, flip flag
		mp_sharedMemory->m_collisionRenderIteratorConVar.wait(m_collisionRenderIteratorUniqueLock);

		mp_sharedMemory->m_threadWaitingFlag = false;
	}

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_collisionRenderIteratorUniqueLock.unlock();

	m_frameWritingIsComplete = false;

	// Write next frame
	while (m_frameWritingIsComplete == false)
	{
		// Check to see if there are scene objects to write in
		m_collisionRenderIteratorUniqueLock.lock();
		m_writeSpritesIntoBuffer = mp_sharedMemory->m_collisionRenderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
		m_collisionRenderIteratorUniqueLock.unlock();

		// While there are scene objects to write
		while (m_writeSpritesIntoBuffer)
		{
			// Write scene object into buffer
			WriteIntoBuffer((*mp_sharedMemory->m_collisionRenderIterator)->GetCollisionRenderInfoRef());

			// Move to next scene object
			++mp_sharedMemory->m_collisionRenderIterator;

			// Check to see if there are scene objects to write in
			m_collisionRenderIteratorUniqueLock.lock();
			m_writeSpritesIntoBuffer = mp_sharedMemory->m_collisionRenderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
			m_collisionRenderIteratorUniqueLock.unlock();
		}

		// Check to see if all scene objects have been written in (frame write is complete)
		m_collisionRenderIteratorUniqueLock.lock();
		m_frameWritingIsComplete = mp_sharedMemory->m_collisionRenderIterator == mr_nullIterator;
		m_collisionRenderIteratorUniqueLock.unlock();
	}

	// Swap buffers and reset the writing buffer
	{
		// Swap
		mp_bufferSwapper = mp_bufferForReading;
		mp_bufferForReading = mp_bufferForWriting;
		mp_bufferForWriting = mp_bufferSwapper;

		// Reset
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
		{
			mp_bufferForWriting[m_reusableIterator].ResetCell(mr_nullIterator);
		}
	}

	// Check collision(s) and render frame (generated above)
	{
		// For each cell
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
		{
			switch (mp_bufferForReading[m_reusableIterator].m_state)
			{
			case BufferCell::State::Collision:
			{
				(*mp_bufferForReading[m_reusableIterator].m_objectsInCellIterators[Consts::NO_VALUE])->Collision(*(*mp_bufferForReading[m_reusableIterator].m_objectsInCellIterators[Consts::OFF_BY_ONE]));
				(*mp_bufferForReading[m_reusableIterator].m_objectsInCellIterators[Consts::OFF_BY_ONE])->Collision(*(*mp_bufferForReading[m_reusableIterator].m_objectsInCellIterators[Consts::NO_VALUE]));
			}
			// NOTE/WARNING: Falls through into snake coloring
			case BufferCell::State::Snake:
			{
				mp_textBuffer[m_reusableIterator].Attributes = m_colorWhite;
			}
			break;
			case BufferCell::State::Empty:
			{
				mp_textBuffer[m_reusableIterator].Attributes = NULL;
			}
			break;
			case BufferCell::State::Food:
			{
				mp_textBuffer[m_reusableIterator].Attributes = BACKGROUND_RED;
			}
			break;
			}
		}

		WriteConsoleOutput(*mp_windowHandle, mp_textBuffer, m_screenBufferCR, m_topLeftCellCR, &m_writeRegionRect);
	}
}
#pragma endregion

#pragma region Private Functionality
void CollisionRenderManager::WriteIntoBuffer(const Structure::CollisionRenderInfo& _collisionRenderInfo)
{
	// For each node
	for (m_nodeIterator = _collisionRenderInfo.m_nodes.begin(); m_nodeIterator != _collisionRenderInfo.m_nodes.end(); ++m_nodeIterator)
	{
		// Write node into buffer
		WriteIntoBufferCell(_collisionRenderInfo);
	}
}
void CollisionRenderManager::WriteIntoBufferCell(const Structure::CollisionRenderInfo& _collisionRenderInfo)
{
	mp_bufferCell = &mp_bufferForWriting[((*m_nodeIterator)->m_position.m_y * m_screenBufferCR.X) + (*m_nodeIterator)->m_position.m_x];
	
	mp_bufferCell->m_objectsInCellIterators[mp_bufferCell->m_objectInCellIndex++] = mp_sharedMemory->m_collisionRenderIterator;

	// What is currently in the cell
	switch (mp_bufferCell->m_state)
	{
		// Do nothing, shouldn't happen
	//case BufferCell::State::Collision:

	case BufferCell::State::Empty:
		mp_bufferCell->m_state = (_collisionRenderInfo.m_objectType == Enums::ObjectType::Food) ? BufferCell::State::Food : BufferCell::State::Snake;
	break;
	case BufferCell::State::Food:
	case BufferCell::State::Snake:
		mp_bufferCell->m_state = BufferCell::State::Collision;
	break;
	}
	
	//const char* BODY_CHAR = "_";
	//
	//// Write character into buffer
	//memcpy(&mp_bufferForWriting[(m_writePosition->m_y * m_screenBufferCR.X) + m_writePosition->m_x].m_character, BODY_CHAR, Consts::OFF_BY_ONE);
}
#pragma endregion

#pragma region Destruction
CollisionRenderManager::~CollisionRenderManager()
{
	delete[] mp_bufferForReading;
	delete[] mp_bufferForWriting;
	delete[] mp_textBuffer;
	delete mp_sharedMemory;
}
#pragma endregion