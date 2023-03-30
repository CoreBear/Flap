#pragma region Includes
#include "RenderManager.h"

#include "Consts.H"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "SharedMemory.h"
#include <Windows.h>
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(HANDLE& _windowHandle, SharedMemory& _sharedMemory, const Structure::Vector2<int>& _bufferSize) :
	m_frameWritingIsComplete(false),
	m_writeSpritesIntoBuffer(false),
	mpp_renderBufferSwapper(nullptr),
	mp_windowHandle(&_windowHandle), 
	m_bufferSize(_bufferSize.m_x * _bufferSize.m_y),
	m_numberOfCharactersToErase(m_bufferSize - Consts::OFF_BY_ONE),
	m_reusableIterator(Consts::NO_VALUE), 
	mr_nullIterator(_sharedMemory.GetNullIteratorRef()),
	mp_sharedMemory(&_sharedMemory),
	m_renderIteratorUniqueLock(_sharedMemory.GetSpriteWriteInIteratorMutexRef())
{
	int numberOfWindowColumns = _bufferSize.m_x;
	int numberOfWindowRows = _bufferSize.m_y;

	mpp_renderBufferForRendering = new char[m_bufferSize];
	memset(mpp_renderBufferForRendering, ' ', m_numberOfCharactersToErase);

	mpp_renderBufferForWriting = new char[m_bufferSize];
	memset(mpp_renderBufferForWriting, ' ', m_numberOfCharactersToErase);

	mp_textBuffer = new CHAR_INFO[m_bufferSize];

	// NOTE: No longer drawing this way
	//for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	//{
	//	mp_textBuffer[m_reusableIterator].Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	//}

	m_screenBufferCR.X = numberOfWindowColumns;
	m_screenBufferCR.Y = numberOfWindowRows;
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);

	m_writeRegionRect.Bottom = static_cast<SHORT>(numberOfWindowRows - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(numberOfWindowColumns - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_renderIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	m_renderIteratorUniqueLock.lock();

	// If Scene thread is already waiting, release the Render thread
	if (mp_sharedMemory->m_threadWaitingFlag)
	{
		mp_sharedMemory->m_renderIteratorConVar.notify_one();
	}

	// If Scene thread is not already waiting, flip flag and wait
	else
	{
		mp_sharedMemory->m_threadWaitingFlag = true;

		// When Scene thread releases this (Render) thread, flip flag
		mp_sharedMemory->m_renderIteratorConVar.wait(m_renderIteratorUniqueLock);

		mp_sharedMemory->m_threadWaitingFlag = false;
	}

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_renderIteratorUniqueLock.unlock();

	m_frameWritingIsComplete = false;

	// Draw next frame (write sprites to buffer)
	while (m_frameWritingIsComplete == false)
	{
		// Check to see if there are sprites to write
		m_renderIteratorUniqueLock.lock();
		m_writeSpritesIntoBuffer = mp_sharedMemory->m_renderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
		m_renderIteratorUniqueLock.unlock();

		// While there are sprites to write
		while (m_writeSpritesIntoBuffer)
		{
			// Write sprite(s)
			WriteSpriteIntoBuffer((*mp_sharedMemory->m_renderIterator)->GetSpriteInfoRef());

			// Move to next object (which holds sprite)
			++mp_sharedMemory->m_renderIterator;

			// Check to see if there are sprites to write
			m_renderIteratorUniqueLock.lock();
			m_writeSpritesIntoBuffer = mp_sharedMemory->m_renderIterator != mp_sharedMemory->GetSceneObjectsIteratorRef();
			m_renderIteratorUniqueLock.unlock();
		}

		// Check to see if all sprites have been written (frame draw is complete)
		m_renderIteratorUniqueLock.lock();
		m_frameWritingIsComplete = mp_sharedMemory->m_renderIterator == mr_nullIterator;
		m_renderIteratorUniqueLock.unlock();
	}

	// Swap buffers and erase the writing buffer
	{
		// Swap
		mpp_renderBufferSwapper = mpp_renderBufferForRendering;
		mpp_renderBufferForRendering = mpp_renderBufferForWriting;
		mpp_renderBufferForWriting = mpp_renderBufferSwapper;

		// Erase
		memset(mpp_renderBufferForWriting, ' ', m_numberOfCharactersToErase);
	}

	// Render frame (the one drawn above)
	{

		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
		{
			mp_textBuffer[m_reusableIterator].Char.UnicodeChar = mpp_renderBufferForRendering[m_reusableIterator];
			
			// If this is a body cell
			const char* CHECK_CHAR = " ";
			if (strcmp((const char*)&mp_textBuffer[m_reusableIterator].Char.UnicodeChar, CHECK_CHAR) != Consts::NO_VALUE)
			{
				mp_textBuffer[m_reusableIterator].Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			}

			// If this is not a body cell
			else
			{
				mp_textBuffer[m_reusableIterator].Attributes = NULL;
			}
		}

		WriteConsoleOutput(*mp_windowHandle, mp_textBuffer, m_screenBufferCR, m_topLeftCellCR, &m_writeRegionRect);
	}
}
#pragma endregion

#pragma region Private Functionality
void RenderManager::WriteConnectorsIntoBuffer(bool _positive, int& _dimenstionToUpdate)
{
	if (_positive)
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < (*m_bodyNodeIterator)->m_numberOfConnectorsToNextNode; m_reusableIterator++)
		{
			_dimenstionToUpdate += m_reusableIterator;

			// Write connector into buffer
			WriteIntoBuffer();
		}
	}
	else
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < (*m_bodyNodeIterator)->m_numberOfConnectorsToNextNode; m_reusableIterator++)
		{
			_dimenstionToUpdate -= m_reusableIterator;

			// Write connector into buffer
			WriteIntoBuffer();
		}
	}
}
void RenderManager::WriteIntoBuffer()
{
	const char* BODY_CHAR = "_";
	size_t sizeOfChar = 1;

	// Write into buffer
	memcpy(&mpp_renderBufferForWriting[(m_nodePosition.m_y * m_screenBufferCR.X) + m_nodePosition.m_x], BODY_CHAR, sizeOfChar);

}
void RenderManager::WriteSpriteIntoBuffer(const Structure::SpriteInfo& _spriteInfo)
{
	// For each node
	for (m_bodyNodeIterator = _spriteInfo.m_bodyNodes.begin(); m_bodyNodeIterator != _spriteInfo.m_bodyNodes.end(); ++m_bodyNodeIterator)
	{
		// Store the node's position
		m_nodePosition = (*m_bodyNodeIterator)->m_position;

		// Write node into buffer
		WriteIntoBuffer();

		switch ((*m_bodyNodeIterator)->m_directionToNextPosition)
		{
		case Enums::Direction::Down:
			WriteConnectorsIntoBuffer(true, m_nodePosition.m_y);
			break;
		case Enums::Direction::Left:
			WriteConnectorsIntoBuffer(false, m_nodePosition.m_x);
			break;
		case Enums::Direction::Right:
			WriteConnectorsIntoBuffer(true, m_nodePosition.m_x);
			break;
		case Enums::Direction::Up:
			WriteConnectorsIntoBuffer(false, m_nodePosition.m_y);
			break;

		// NOTE: If execution makes it here, this is the last node
		default:
			break;
		}
	}
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete[] mpp_renderBufferForRendering;
	delete[] mpp_renderBufferForWriting;
	delete[] mp_textBuffer;
	delete mp_sharedMemory;
}
#pragma endregion