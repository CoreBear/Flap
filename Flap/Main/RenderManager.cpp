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
	mp_spriteRow(nullptr),
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
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	{
		mp_textBuffer[m_reusableIterator].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

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
		}

		WriteConsoleOutput(*mp_windowHandle, mp_textBuffer, m_screenBufferCR, m_topLeftCellCR, &m_writeRegionRect);
	}
}
#pragma endregion

#pragma region Private Functionality
void RenderManager::WriteSpriteIntoBuffer(const Structure::SpriteInfo& _spriteInfo)
{
	// Copy each sprite row into the buffer
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _spriteInfo.m_spriteHeight; m_reusableIterator++)
	{
		mp_spriteRow = _spriteInfo.mppp_sprite[_spriteInfo.m_animationKeyFrameIndexToRender][m_reusableIterator];
		memcpy(&mpp_renderBufferForWriting[((_spriteInfo.m_position.m_y * m_screenBufferCR.X) + (m_reusableIterator * m_screenBufferCR.Y)) + _spriteInfo.m_position.m_x], mp_spriteRow, strlen(mp_spriteRow));
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