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
	mr_sharedRender(_sharedRender)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _sharedRender.m_bufferSize; m_reusableIterator++)
	{
		mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.Y = static_cast<SHORT>(Consts::NO_VALUE);
	
	m_writeRegionRect.Bottom = static_cast<SHORT>(_sharedRender.m_bufferHW.Y - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(_sharedRender.m_bufferHW.X - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	mr_sharedRender.m_frameBufferMutex.lock();

	if (mr_sharedRender.m_somethingToRender)
	{
		// For each cell
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedRender.m_bufferSize; m_reusableIterator++)
		{
			mp_renderBuffer[m_reusableIterator].Attributes = mr_sharedRender.mp_frameBuffer[m_reusableIterator].m_colorBFGround;
			mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = mr_sharedRender.mp_frameBuffer[m_reusableIterator].m_character;
		}

		// Render from buffer
		WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_renderBuffer, mr_sharedRender.m_bufferHW, m_topLeftCellCR, &m_writeRegionRect);

		mr_sharedRender.ResetFrameBuffer();
	}

	mr_sharedRender.m_frameBufferMutex.unlock();
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete[] mp_renderBuffer;
}
#pragma endregion