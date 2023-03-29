#pragma region Includes
#include "RenderManager.h"

#include "AtomicMemory.h"
#include "Consts.H"
#include "ObjectManager.h"

#include <Windows.h>
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(AtomicMemory& _atomicMemory, HANDLE& _windowHandle) : mp_atomicMemory(&_atomicMemory), mp_spriteInfo(nullptr), mp_windowHandle(&_windowHandle)
{
	mp_textBuffer = new CHAR_INFO[mp_atomicMemory->m_bufferSize];

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mp_atomicMemory->m_bufferSize; m_reusableIterator++)
	{
		mp_textBuffer[m_reusableIterator].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

	m_screenBufferCR.X = mp_atomicMemory->m_numberOfWindowColumns;
	m_screenBufferCR.Y = mp_atomicMemory->m_numberOfWindowRows;
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);

	m_writeRegionRect.Bottom = static_cast<SHORT>(mp_atomicMemory->m_numberOfWindowRows - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(mp_atomicMemory->m_numberOfWindowColumns - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	std::unique_lock<std::mutex> renderBufferUniqueLock(mp_atomicMemory->m_renderBufferMutex);

	// Draw next frame
	WriteSpritesIntoBuffer(mp_atomicMemory->GetConstSceneObjectsListRef());

	// Swap buffers
	mp_atomicMemory->SwapBuffersAndClearScratch();

	// Render next (now this) frame
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mp_atomicMemory->m_bufferSize; m_reusableIterator++)
		{
			mp_textBuffer[m_reusableIterator].Char.UnicodeChar = mp_atomicMemory->mpp_renderBuffer[m_reusableIterator];
		}

		WriteConsoleOutput(*mp_windowHandle, mp_textBuffer, m_screenBufferCR, m_topLeftCellCR, &m_writeRegionRect);
	}

	// Wait for next frame
	mp_atomicMemory->m_renderBufferConVar.wait(renderBufferUniqueLock);
}
#pragma endregion

#pragma region Private Functionality
void RenderManager::WriteSpritesIntoBuffer(const std::list<SceneObject*>& _sceneObjectsList) 
{
	for (m_sceneObjectsIterator = _sceneObjectsList.begin(); m_sceneObjectsIterator != _sceneObjectsList.end(); m_sceneObjectsIterator++)
	{
		mp_spriteInfo = &(*m_sceneObjectsIterator)->GetConstSpriteInfoRef();
		mp_atomicMemory->AddSpriteToScratch(mp_spriteInfo->mppp_sprite[mp_spriteInfo->m_animationKeyFrameIndexToRender], mp_spriteInfo->m_spriteHeight, (*m_sceneObjectsIterator)->GetConstPositionRef());
	}
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete mp_atomicMemory;
	delete[] mp_textBuffer;
}
#pragma endregion