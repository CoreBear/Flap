#pragma region Includes
#include "InputReceiver.h"

#include "SharedMemory.h"
#pragma endregion

#pragma region Static Initializatino
SharedMemory* InputReceiver::sp_sharedMemory = nullptr;
#pragma endregion

#pragma region Protected Functionality
void InputReceiver::HandleInput()
{
	mp_inputQueueMutex->lock();

	while (mp_inputQueue->empty() == false)
	{
		m_currentInput = mp_inputQueue->front();
		mp_inputQueue->pop();
		mp_inputQueueMutex->unlock();

		switch ((Enums::InputName)m_currentInput.m_inputIndex)
		{
		case Enums::InputName::Down:
			InputDown(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Left:
			InputLeft(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Right:
			InputRight(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Up:
			InputUp(m_currentInput.m_inputPressState);
			break;
		}

		mp_inputQueueMutex->lock();
	}

	mp_inputQueueMutex->unlock();
}
#pragma endregion