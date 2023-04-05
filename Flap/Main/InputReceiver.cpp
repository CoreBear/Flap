#pragma region Includes
#include "InputReceiver.h"

#include "SharedInput.h"
#pragma endregion

#pragma region Static Initialization
SharedInput* InputReceiver::sp_sharedInput = nullptr;
#pragma endregion

#pragma region Protected Functionality
void InputReceiver::HandleInput()
{
	sp_sharedInput->m_inputQueueMutex.lock();

	while (sp_sharedInput->m_inputQueue[m_playerIndex].empty() == false)
	{
		m_currentInput = sp_sharedInput->m_inputQueue[m_playerIndex].front();
		sp_sharedInput->m_inputQueue[m_playerIndex].pop();
		//sp_sharedInput->m_inputQueueMutex.unlock();

		switch ((Enums::Direction)m_currentInput.m_inputIndex)
		{
		case Enums::Direction::Down:
			InputDown(m_currentInput.m_inputPressState);
			break;
		case Enums::Direction::Left:
			InputLeft(m_currentInput.m_inputPressState);
			break;
		case Enums::Direction::Right:
			InputRight(m_currentInput.m_inputPressState);
			break;
		case Enums::Direction::Up:
			InputUp(m_currentInput.m_inputPressState);
			break;
		}

		//sp_sharedInput->m_inputQueueMutex.lock();
	}

	sp_sharedInput->m_inputQueueMutex.unlock();
}
#pragma endregion