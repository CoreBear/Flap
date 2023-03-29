#pragma region Includes
#include "InputManager.h"

#include "AtomicMemory.h"
#include "Consts.h"
#include "SceneManager.h"
#pragma endregion

#pragma region Initialization
InputManager::InputManager(AtomicMemory& _atomicMemory, HANDLE& _windowHandle) :
	mp_inputPressStates(new Enums::InputPressState[Consts::NUMBER_OF_INPUTS]),
	m_windowHandle(GetStdHandle(STD_INPUT_HANDLE)),
	mr_inputQueue(_atomicMemory.GetInputQueueRef()),
	mp_deadFramesTargetFrames(new unsigned int[Consts::NUMBER_OF_INPUTS])
{
	// Reset all input
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::NUMBER_OF_INPUTS; m_reusableIterator_1++)
	{
		mp_inputPressStates[m_reusableIterator_1] = Enums::InputPressState::Released;
	}

	m_bufferLength = sizeof(INPUT_RECORD) * UCHAR_MAX;
}
#pragma endregion

#pragma region Updates
void InputManager::Update()
{
	//https://learn.microsoft.com/en-us/windows/console/readconsoleinput
	// NOTE: If this returns 0, error occurred
	ReadConsoleInput(m_windowHandle, m_inputRecords, m_bufferLength, &m_numberOfEventsRead);

	// For each record
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < m_numberOfEventsRead; m_reusableIterator_1++)
	{
		//https://learn.microsoft.com/en-us/windows/console/input-record-str
		// NOTE: Records can be focus, key, menu, mouse, window buffer
		// Only handle key input
		if (m_inputRecords[m_reusableIterator_1].EventType == KEY_EVENT) 
		{
			// For each input being watched
			for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2  < Consts::NUMBER_OF_INPUTS; m_reusableIterator_2++)
			{
				// If input that caused event is an input being watched
				if (m_inputRecords[m_reusableIterator_1].Event.KeyEvent.wVirtualKeyCode == Consts::INPUTS[m_reusableIterator_2])
				{
					ReadAndEnqueueInput(m_inputRecords[m_reusableIterator_1].Event.KeyEvent);

					// Stop looking for input
					break;
				}
			}
		}
	}
}
#pragma endregion

#pragma region Private Functionality
void InputManager::ReadAndEnqueueInput(const KEY_EVENT_RECORD& _inputInfo)
{
	// If input is pressed down
	if (_inputInfo.bKeyDown)
	{
		switch (mp_inputPressStates[m_reusableIterator_2])
		{
			// Do nothing
		//case Enums::InputPressState::Held:

		case Enums::InputPressState::Click:
		case Enums::InputPressState::Released:
		{
			mp_inputPressStates[m_reusableIterator_2] = Enums::InputPressState::PressedThisFrame;
		}
		break;
		case Enums::InputPressState::Dead:
		{
			if (SceneManager::s_simFrameCount >= mp_deadFramesTargetFrames[m_reusableIterator_2])
			{
				mp_inputPressStates[m_reusableIterator_2] = Enums::InputPressState::Held;
			}
		}
		// NOTE/WARNING: Notice this return! Dead frames shouldn't be enqueues
		return;
		
		case Enums::InputPressState::PressedThisFrame:
		{
			mp_inputPressStates[m_reusableIterator_2] = Enums::InputPressState::Dead;

			// Arbitrary value, represents click-to-hold number of frames
			const unsigned int NUMBER_OF_DEAD_FRAMES = 15;
			mp_deadFramesTargetFrames[m_reusableIterator_2] = SceneManager::s_simFrameCount + NUMBER_OF_DEAD_FRAMES;
		}
		break;
		}
	}

	// If input is released
	else
	{
		switch (mp_inputPressStates[m_reusableIterator_2])
		{
			// Cannot occur
		//case Enums::InputPressState::Click:
		//case Enums::InputPressState::Released:

		case Enums::InputPressState::Dead:
		case Enums::InputPressState::PressedThisFrame:
		{
			mp_inputPressStates[m_reusableIterator_2] = Enums::InputPressState::Click;
		}
		break;
		case Enums::InputPressState::Held:
		{
			mp_inputPressStates[m_reusableIterator_2] = Enums::InputPressState::Released;
		}
		break;
		}
	}

	// Add values to input
	m_newInput.m_input = static_cast<short>(_inputInfo.wVirtualKeyCode);
	m_newInput.m_inputPressState = mp_inputPressStates[m_reusableIterator_2];

	// Add input to queue
	mr_inputQueue.push(m_newInput);
}
#pragma endregion

#pragma region Destruction
InputManager::~InputManager()
{
	delete[] mp_inputPressStates;
	delete[] mp_deadFramesTargetFrames;
}
#pragma endregion