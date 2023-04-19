#pragma region Includes
#include "RenderManager.h"

#include "BufferCell.h"
#include "SceneObject.h"
#include "SharedRender.h"

#include <mutex>
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(const HANDLE& _outputWindowHandle, SharedRender& _sharedRender) :
	mp_renderBuffer(new CHAR_INFO[_sharedRender.m_bufferSize]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator(Consts::NO_VALUE),
	mr_sharedRender(_sharedRender),
	m_frameBufferUniqueLock(mr_sharedRender.m_frameBufferMutex)
{
	m_frameBufferUniqueLock.unlock();

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _sharedRender.m_bufferSize; m_reusableIterator++)
	{
		mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_topLeftCellDimensions.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellDimensions.Y = static_cast<SHORT>(Consts::NO_VALUE);
	
	m_writeRegionRect.Bottom = static_cast<SHORT>(_sharedRender.m_frameBufferDimensions.Y - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(_sharedRender.m_frameBufferDimensions.X - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	m_frameBufferUniqueLock.lock();
	mr_sharedRender.m_frameBufferConVar.wait(m_frameBufferUniqueLock);

	// For each cell
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedRender.m_bufferSize; m_reusableIterator++)
	{
		mp_renderBuffer[m_reusableIterator].Attributes = mr_sharedRender.mp_frameBuffer[m_reusableIterator].m_colorBFGround;
		mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = mr_sharedRender.mp_frameBuffer[m_reusableIterator].m_character;
	}

	// Render from buffer
	WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_renderBuffer, mr_sharedRender.m_frameBufferDimensions, m_topLeftCellDimensions, &m_writeRegionRect);

	mr_sharedRender.ResetFrameBuffer();

	m_frameBufferUniqueLock.unlock();
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete[] mp_renderBuffer;
}
#pragma endregion