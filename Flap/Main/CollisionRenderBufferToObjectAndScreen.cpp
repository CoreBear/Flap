#pragma region Includes
#include "CollisionRenderBufferToObjectAndScreen.h"

#include "BufferCell.h"
#include "SceneObject.h"
#include "SharedCollisionRender.h"
#pragma endregion

#pragma region Initialization
CollisionRenderBufferToObjectAndScreen::CollisionRenderBufferToObjectAndScreen(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender) :
	mp_textBuffer(new CHAR_INFO[_sharedCollisionRender.m_bufferSize]),
	OUTPUT_WINDOW_HANDLE(_outputWindowHandle),
	m_reusableIterator(Consts::NO_VALUE),
	mr_sharedCollisionRender(_sharedCollisionRender)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < _sharedCollisionRender.m_bufferSize; m_reusableIterator++)
	{
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = Consts::EMPTY_SPACE_CHAR;
	}

	m_topLeftCellCR.X = static_cast<SHORT>(Consts::NO_VALUE);
	m_topLeftCellCR.Y = static_cast<SHORT>(Consts::NO_VALUE);
	
	m_writeRegionRect.Bottom = static_cast<SHORT>(_sharedCollisionRender.SCREEN_BUFFER_CR.Y - Consts::OFF_BY_ONE);
	m_writeRegionRect.Left = static_cast<SHORT>(Consts::NO_VALUE);
	m_writeRegionRect.Right = static_cast<SHORT>(_sharedCollisionRender.SCREEN_BUFFER_CR.X - Consts::OFF_BY_ONE);
	m_writeRegionRect.Top = static_cast<SHORT>(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Public Functionality
void CollisionRenderBufferToObjectAndScreen::GameUpdate()
{
	// For each cell
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedCollisionRender.m_bufferSize; m_reusableIterator++)
	{
		if (mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].m_cellState == Enums::CellState::Collision)
		{
			// Get column and row
			m_collisionCellCR.m_x = m_reusableIterator % mr_sharedCollisionRender.SCREEN_BUFFER_CR.X;
			m_collisionCellCR.m_y = m_reusableIterator / mr_sharedCollisionRender.SCREEN_BUFFER_CR.X;

			// Send each object, the other object and the collision cell's column and row
			(*mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE])->Collision(&(*mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE])->GetCollisionPackageRef(), m_collisionCellCR);
			(*mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::OFF_BY_ONE])->Collision(&(*mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].mp_objectsInCellIterators[Consts::NO_VALUE])->GetCollisionPackageRef(), m_collisionCellCR);
		}

		mp_textBuffer[m_reusableIterator].Attributes = mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].m_colorBFGround;
		mp_textBuffer[m_reusableIterator].Char.UnicodeChar = mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].m_character;
	}

	// Render from buffer
	WriteConsoleOutput(OUTPUT_WINDOW_HANDLE, mp_textBuffer, mr_sharedCollisionRender.SCREEN_BUFFER_CR, m_topLeftCellCR, &m_writeRegionRect);

	// Reset the buffer
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedCollisionRender.m_bufferSize; m_reusableIterator++)
	{
		mr_sharedCollisionRender.mp_frameBuffer[m_reusableIterator].ResetCell();
	}
}
void CollisionRenderBufferToObjectAndScreen::OverlayUpdate()
{
	return;
}
#pragma endregion

#pragma region Destruction
CollisionRenderBufferToObjectAndScreen::~CollisionRenderBufferToObjectAndScreen()
{
	delete[] mp_textBuffer;
}
#pragma endregion