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

	while (sp_sharedInput->m_inputQueue[m_readIndex].empty() == false)
	{
		m_currentInput = sp_sharedInput->m_inputQueue[m_readIndex].front();
		sp_sharedInput->m_inputQueue[m_readIndex].pop();

		//sp_sharedInput->m_inputQueueMutex.unlock();

		switch ((Enums::InputName)m_currentInput.m_inputIndexOrCharacter)
		{
		case Enums::InputName::Accept:
			InputAccept(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Down:
			InputDown(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Left:
			InputLeft(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Pause:
			InputPause(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Right:
			InputRight(m_currentInput.m_inputPressState);
			break;
		case Enums::InputName::Up:
			InputUp(m_currentInput.m_inputPressState);
			break;

			// High Score Characters
		default:
			InputCharacter(m_currentInput.m_inputIndexOrCharacter);
			break;
		}

		//sp_sharedInput->m_inputQueueMutex.lock();
	}

	sp_sharedInput->m_inputQueueMutex.unlock();
}
#pragma endregion