#pragma region Includes
#include "SharedCollisionRender.h"

#include "BufferCell.h"
#include "Snake.h"

#include <Windows.h>
#pragma endregion

#pragma region Initialization
SharedCollisionRender::SharedCollisionRender(const COORD& _bufferSize) :
	m_bufferSize(_bufferSize.X * _bufferSize.Y),
	m_threadWaitingFlag(false),
	mp_frameBuffer(new BufferCell[m_bufferSize]),
	SCREEN_BUFFER_CR(_bufferSize)
{
	Snake::AssignSharedCollisionRender(*this);

	for (int index = Consts::NO_VALUE; index < m_bufferSize; index++)
	{
		mp_frameBuffer[index].ResetCell();
	}
}
#pragma endregion

#pragma region Destruction
SharedCollisionRender::~SharedCollisionRender()
{
	delete[] mp_frameBuffer;
}
#pragma endregion