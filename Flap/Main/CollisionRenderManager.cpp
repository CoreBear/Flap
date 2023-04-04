#pragma region Includes
#include "CollisionRenderManager.h"

#include "SharedMemory.h"
#pragma endregion

#pragma region Initialization
CollisionRenderManager::CollisionRenderManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory) :
	mr_bufferSize(_sharedMemory.GetBufferSizeRef()),
	mp_textBuffer(new CHAR_INFO[mr_bufferSize]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator(Consts::NO_VALUE),
	mr_sharedMemory(_sharedMemory)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_bufferSize; m_reusableIterator++)
	{
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.Y = static_cast<SHORT>(Consts::NO_VALUE);
	
	m_writeRegionRect.Bottom = static_cast<SHORT>(_sharedMemory.SCREEN_BUFFER_CR.Y - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(_sharedMemory.SCREEN_BUFFER_CR.X - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Public Functionality
void CollisionRenderManager::GameUpdate()
{
	// For each cell
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_bufferSize; m_reusableIterator++)
	{
		if (mr_sharedMemory.mp_frameBuffer[m_reusableIterator].m_cellState == Enums::CellState::Collision)
		{
			// Get column and row
			m_collisionCellCR.m_x = m_reusableIterator % mr_sharedMemory.SCREEN_BUFFER_CR.X;
			m_collisionCellCR.m_y = m_reusableIterator / mr_sharedMemory.SCREEN_BUFFER_CR.X;

			// Send each object, the other object and the collision cell's column and row
			//(*mr_sharedMemory.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE])->Collision(*(*mr_sharedMemory.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE]), m_collisionCellCR);
			//(*mr_sharedMemory.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE])->Collision(*(*mr_sharedMemory.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE]), m_collisionCellCR);
		}

		mp_textBuffer[m_reusableIterator].Attributes = mr_sharedMemory.mp_frameBuffer[m_reusableIterator].m_colorBFGround;
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = mr_sharedMemory.mp_frameBuffer[m_reusableIterator].m_character;
	}

	// Render from buffer
	WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_textBuffer, mr_sharedMemory.SCREEN_BUFFER_CR, m_topLeftCellCR, &m_writeRegionRect);

	// Reset the buffer
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_bufferSize; m_reusableIterator++)
	{
		mr_sharedMemory.mp_frameBuffer[m_reusableIterator].ResetCell();
	}
}
void CollisionRenderManager::OverlayUpdate()
{
	return;
}
#pragma endregion

#pragma region Destruction
CollisionRenderManager::~CollisionRenderManager()
{
	delete[] mp_textBuffer;
}
#pragma endregion