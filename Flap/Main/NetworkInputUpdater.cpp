#pragma region Includes
#include "NetworkInputUpdater.h"

#include "Consts.h"
#include "SharedInput.h"
#include "SharedNetwork.h"

#include <mutex>
#include <queue>
#pragma endregion

#pragma region Updates
void NetworkInputUpdater::Update()
{
	mr_sharedInput.m_inputQueueMutex.lock();

	// NOTE: Only handle player 1's input index
	while (mr_sharedInput.m_inputQueue[Consts::NO_VALUE].empty() == false)
	{
		m_currentInput = mr_sharedInput.m_inputQueue[Consts::NO_VALUE].front();
		mr_sharedInput.m_inputQueue[Consts::NO_VALUE].pop();

		mr_sharedInput.m_inputQueueMutex.unlock();

		if (m_currentInput.m_inputPressState == Enums::InputPressState::PressedThisFrame)
		{
			mr_sharedNetwork.m_mostRecentClientInputMutex.lock();
			mr_sharedNetwork.m_mostRecentClientInput == m_currentInput.m_inputIndexOrCharacter;
			mr_sharedNetwork.m_mostRecentClientInputMutex.unlock();
		}

		mr_sharedInput.m_inputQueueMutex.lock();
	}

	// Empty out player 2's input index, because it's not actually being handled
	while (mr_sharedInput.m_inputQueue[Consts::OFF_BY_ONE].empty() == false)
	{
		mr_sharedInput.m_inputQueue[Consts::OFF_BY_ONE].pop();
	}

	mr_sharedInput.m_inputQueueMutex.unlock();
}
#pragma endregion