#pragma region Includes
#include "InputManager.h"

#include "Consts.h"
#include "SceneManager.h"
#include "SharedInput.h"
#pragma endregion

#pragma region Initialization
InputManager::InputManager(SharedInput& _sharedInput) :
	BUFFER_LENGTH(Consts::MAX_NUMBER_OF_PLAYERS* Consts::NUMBER_OF_INPUTS),
	INPUT_WINDOW_HANDLE(GetStdHandle(STD_INPUT_HANDLE)),
	mpp_inputPressStates(new Enums::InputPressState* [Consts::MAX_NUMBER_OF_PLAYERS]),
	m_numberOfEventsRead(Consts::NO_VALUE),
	m_reusableIterator_1(Consts::NO_VALUE),
	m_reusableIterator_3(Consts::NO_VALUE),
	mr_sharedInput(_sharedInput),
	mpp_deadFramesTargetFrames(new unsigned int*[Consts::MAX_NUMBER_OF_PLAYERS])
{
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS; m_reusableIterator_1++)
	{
		mpp_inputPressStates[m_reusableIterator_1] = new Enums::InputPressState[Consts::NUMBER_OF_INPUTS];
		mpp_deadFramesTargetFrames[m_reusableIterator_1] = new unsigned int[Consts::NUMBER_OF_INPUTS];

		for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < Consts::NUMBER_OF_INPUTS; m_reusableIterator_2++)
		{
			mpp_inputPressStates[m_reusableIterator_1][m_reusableIterator_2] = Enums::InputPressState::Released;
			mpp_deadFramesTargetFrames[m_reusableIterator_1][m_reusableIterator_2] = static_cast<unsigned int>(Consts::NO_VALUE);
		}
	}
}
#pragma endregion

#pragma region Updates
void InputManager::Update()
{
	//https://learn.microsoft.com/en-us/windows/console/readconsoleinput
	// NOTE: If this returns 0, error occurred
	//ReadConsoleInput(m_windowHandle, m_inputRecords, BUFFER_LENGTH, reinterpret_cast<LPDWORD>(&m_numberOfEventsRead));

	// Read input
	// NOTE: This call blocks, which is fine, since input is on its own thread
	ReadConsoleInput(INPUT_WINDOW_HANDLE, m_inputRecords, BUFFER_LENGTH, reinterpret_cast<LPDWORD>(&m_numberOfEventsRead));

	// For each record
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < m_numberOfEventsRead; m_reusableIterator_1++)
	{
		//https://learn.microsoft.com/en-us/windows/console/input-record-str
		// NOTE: Records can be focus, key, menu, mouse, window buffer
		// Only handle key input
		if (m_inputRecords[m_reusableIterator_1].EventType == KEY_EVENT)
		{
			// For each player
			for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < Consts::MAX_NUMBER_OF_PLAYERS; m_reusableIterator_2++)
			{
				// For each player's inputs being watched
				for (m_reusableIterator_3 = Consts::NO_VALUE; m_reusableIterator_3 < Consts::NUMBER_OF_INPUTS; m_reusableIterator_3++)
				{
					// If input that caused event is an input being watched
					if (m_inputRecords[m_reusableIterator_1].Event.KeyEvent.wVirtualKeyCode == Consts::INPUTS[m_reusableIterator_2][m_reusableIterator_3])
					{
						ReadAndEnqueueInput(m_inputRecords[m_reusableIterator_1].Event.KeyEvent);

						// Stop looking for input
						return;
					}
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
		switch (mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3])
		{
			// Do nothing
		//case Enums::InputPressState::Held:

		case Enums::InputPressState::Click:
		case Enums::InputPressState::Released:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::PressedThisFrame;
		}
		break;
		case Enums::InputPressState::Dead:
		{
			if (SceneManager::s_fixedFrameCount == mpp_deadFramesTargetFrames[m_reusableIterator_2][m_reusableIterator_3])
			{
				mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Held;
			}
		}
		return;
		
		// NOTE/WARNING: Notice this return! Dead frames shouldn't be enqueues
		case Enums::InputPressState::PressedThisFrame:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Dead;

			// Arbitrary value, represents click-to-hold number of frames
			const unsigned int NUMBER_OF_DEAD_FRAMES = 15;
			mpp_deadFramesTargetFrames[m_reusableIterator_2][m_reusableIterator_3] = SceneManager::s_fixedFrameCount + NUMBER_OF_DEAD_FRAMES;
		}
		break;
		}
	}

	// If input is released
	else
	{
		switch (mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3])
		{
			// Cannot occur
		//case Enums::InputPressState::Click:
		//case Enums::InputPressState::Released:

		case Enums::InputPressState::Dead:
		case Enums::InputPressState::PressedThisFrame:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Click;
		}
		break;
		case Enums::InputPressState::Held:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Released;
		}
		break;
		}
	}

	// Add values to input
	m_newInput.m_inputIndex = m_reusableIterator_3;
	m_newInput.m_inputPressState = mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3];

	// Add input to queue
	mr_sharedInput.m_inputQueueMutex.lock();
	mr_sharedInput.m_inputQueue[m_reusableIterator_2].push(m_newInput);
	mr_sharedInput.m_inputQueueMutex.unlock();
}
#pragma endregion

#pragma region Destruction
InputManager::~InputManager()
{
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS; m_reusableIterator_1++)
	{
		delete[] mpp_inputPressStates[m_reusableIterator_1];
		delete[] mpp_deadFramesTargetFrames[m_reusableIterator_1];
	}

	delete[] mpp_inputPressStates;
	delete[] mpp_deadFramesTargetFrames;
}
#pragma endregion