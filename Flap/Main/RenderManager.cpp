#pragma region Includes
#include "RenderManager.h"

#include "BufferCell.h"
#include "SceneObject.h"
#include "SharedGame.h"
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame) :
	mp_renderBuffer(new CHAR_INFO[_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x * _sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator_1(Consts::NO_VALUE),
	mr_sharedGame(_sharedGame),
	m_frameBufferUniqueLock(mr_sharedGame.m_frameBufferMutex)
{
	m_frameBufferUniqueLock.unlock();

	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < _sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x * _sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
	{
		mp_renderBuffer[m_reusableIterator_1].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	FRAME_BUFFER_HEIGHT_WIDTH.X = _sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x;
	FRAME_BUFFER_HEIGHT_WIDTH.Y = _sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y;

	m_frameBufferTopLeftPosition.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_frameBufferTopLeftPosition.Y = static_cast<SHORT>(Consts::NO_VALUE);
	
	m_drawRegion.Bottom = static_cast<SHORT>(_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y - Consts::OFF_BY_ONE);
	m_drawRegion.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_drawRegion.Right = static_cast<SHORT>(_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x - Consts::OFF_BY_ONE);
	m_drawRegion.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Updates
void RenderManager::Update()
{
	m_frameBufferUniqueLock.lock();
	mr_sharedGame.m_frameBufferConVar.wait(m_frameBufferUniqueLock);

	m_renderBufferIndex = Consts::NO_VALUE;

	if (mr_sharedGame.m_borderIsRequired)
	{
		// For each cell
		for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < mr_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
		{
			for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < mr_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
			{
				// If border cell
				if (m_reusableIterator_2 < mr_sharedGame.m_gameAreaBounds.m_w						||	// Left bound
					m_reusableIterator_1 < mr_sharedGame.m_gameAreaBounds.m_x						|| 	// Top bound
					m_reusableIterator_2 > mr_sharedGame.m_gameAreaBounds.m_y - Consts::OFF_BY_ONE	||	// Right bound
					m_reusableIterator_1 > mr_sharedGame.m_gameAreaBounds.m_z - Consts::OFF_BY_ONE)		// Bottom bound
				{
					mp_renderBuffer[m_renderBufferIndex].Attributes = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
					mp_renderBuffer[m_renderBufferIndex].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
				}

				// If not border cell
				else
				{
					mp_renderBuffer[m_renderBufferIndex].Attributes = mr_sharedGame.mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_colorBFGround;
					mp_renderBuffer[m_renderBufferIndex].Char.UnicodeChar = mr_sharedGame.mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_character;
				}

				++m_renderBufferIndex;
			}
		}
	}

	// If no border is required
	else
	{
		// For each cell
		for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < mr_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
		{
			for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < mr_sharedGame.FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
			{
				mp_renderBuffer[m_renderBufferIndex].Attributes = mr_sharedGame.mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_colorBFGround;
				mp_renderBuffer[m_renderBufferIndex].Char.UnicodeChar = mr_sharedGame.mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_character;

				++m_renderBufferIndex;
			}
		}
	}

	// Render from buffer
	WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_renderBuffer, FRAME_BUFFER_HEIGHT_WIDTH, m_frameBufferTopLeftPosition, &m_drawRegion);

	mr_sharedGame.ResetFrameBuffer();

	m_frameBufferUniqueLock.unlock();
}
#pragma endregion

#pragma region Destruction
RenderManager::~RenderManager()
{
	delete[] mp_renderBuffer;
}
#pragma endregion