#pragma region Includes
#include "SharedMemory.h"

#include "Snake.h"

#include <Windows.h>
#pragma endregion

#pragma region Initialization
SharedMemory::SharedMemory(const COORD& _bufferSize) :
	m_threadWaitingFlag(false),
	mr_screenBufferCR(_bufferSize)
{
	Snake::AssignSharedMemory(*this);
}
#pragma endregion