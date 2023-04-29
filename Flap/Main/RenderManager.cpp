#pragma region Includes
#include "RenderManager.h"

#include "BufferCell.h"
#include "SceneObject.h"
#include "SharedGame.h"
#pragma endregion

#pragma region Initialization
RenderManager::RenderManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame) :
	mp_renderBuffer(new CHAR_INFO[_sharedGame.m_bufferSize]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator(Consts::NO_VALUE),
	mr_sharedGame(_sharedGame),
	m_frameBufferUniqueLock(mr_sharedGame.m_frameBufferMutex)
{
	m_frameBufferUniqueLock.unlock();

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _sharedGame.m_bufferSize; m_reusableIterator++)
	{
		mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
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

	// For each cell
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.m_bufferSize; m_reusableIterator++)
	{
		mp_renderBuffer[m_reusableIterator].Attributes = mr_sharedGame.mp_frameBuffer[m_reusableIterator].m_colorBFGround;
		mp_renderBuffer[m_reusableIterator].Char.UnicodeChar = mr_sharedGame.mp_frameBuffer[m_reusableIterator].m_character;
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