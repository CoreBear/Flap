#pragma region Includes
#include "SharedMemory.h"

#include "InputReceiver.h"
#pragma endregion

#pragma region Initialization
SharedMemory::SharedMemory() : m_threadWaitingFlag(false)
{
	InputReceiver::AssignSharedMemory(*this);
}
#pragma endregion