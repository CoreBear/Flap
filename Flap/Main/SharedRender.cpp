#pragma region Includes
#include "SharedRender.h"

#include "BufferCell.h"
#include "Snake.h"
#include "Tools.h"

#include <Windows.h>
#pragma endregion

#pragma region Initialization
SharedRender::SharedRender(const COORD& _bufferSize) :
	m_bufferSize(_bufferSize.X * _bufferSize.Y),
	mp_frameBuffer(new BufferCell[m_bufferSize]),
	m_bufferHW(_bufferSize)
{
	Snake::AssignSharedRender(*this);
	Tools::AssignSharedRender(*this);

	ResetFrameBuffer();
}
#pragma endregion

#pragma region Public Functionality
void SharedRender::ResetFrameBuffer()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_bufferSize; m_reusableIterator++)
	{
		mp_frameBuffer[m_reusableIterator].ResetCell();
	}
}
void SharedRender::ResetFrameBufferSynced()
{
	m_frameBufferMutex.lock();

	ResetFrameBuffer();

	m_frameBufferMutex.unlock();
}
#pragma endregion

#pragma region Destruction
SharedRender::~SharedRender()
{
	delete[] mp_frameBuffer;
}
#pragma endregion