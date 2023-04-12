#pragma region Includes
#include "InputManager.h"

#include "Consts.h"
#include "GameManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#pragma endregion

#pragma region Initialization
InputManager::InputManager(SharedGame& _sharedGame, SharedInput& _sharedInput) :
	BUFFER_LENGTH(Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM * Consts::NUMBER_OF_INPUTS),
	INPUT_WINDOW_HANDLE(GetStdHandle(STD_INPUT_HANDLE)),
	mpp_inputPressStates(new Enums::InputPressState* [Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM]),
	m_numberOfEventsRead(Consts::NO_VALUE),
	m_reusableIterator_1(Consts::NO_VALUE),
	m_reusableIterator_2(Consts::NO_VALUE),
	m_reusableIterator_3(Consts::NO_VALUE),
	m_reusableIterator_4(Consts::NO_VALUE),
	mr_sharedGame(_sharedGame),
	mr_sharedInput(_sharedInput),
	mpp_deadFramesTargetFrames(new unsigned int*[Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM])
{
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM; m_reusableIterator_1++)
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
			for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM; m_reusableIterator_2++)
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
		// NOTE/WARNING: Notice the returns! Dead and hold frames frames shouldn't be enqueued
		switch (mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3])
		{
		case Enums::InputPressState::Click:
		case Enums::InputPressState::Released:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::PressedThisFrame;
		}
		break;
		case Enums::InputPressState::Dead:
		{
			if (GameManager::s_fixedFrameCount == mpp_deadFramesTargetFrames[m_reusableIterator_2][m_reusableIterator_3])
			{
				mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Held;
			}
		}
		// NOTE: Fallthrough!
		case Enums::InputPressState::Held:
			return;
		case Enums::InputPressState::PressedThisFrame:
		{
			mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3] = Enums::InputPressState::Dead;

			// Arbitrary value, represents click-to-hold number of frames
			const unsigned int NUMBER_OF_DEAD_FRAMES = 15;
			mpp_deadFramesTargetFrames[m_reusableIterator_2][m_reusableIterator_3] = GameManager::s_fixedFrameCount + NUMBER_OF_DEAD_FRAMES;
		}
		return;
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
	constexpr int PAUSE_INDEX = static_cast<int>(Enums::InputName::Pause);

	// If neither player is trying to pause the game
	if (m_reusableIterator_3 != PAUSE_INDEX)
	{
		mr_sharedGame.m_gameStateMutex.lock();
		switch (mr_sharedGame.m_gameState)
		{
			// Do nothing
		//case Enums::GameState::ExitApp:
		//case Enums::GameState::ExitToMain:
		//case Enums::GameState::ExitToResults:
		//case Enums::GameState::PauseGame:
		//case Enums::GameState::ResumeGame:
		//case Enums::GameState::StartGame:

			// If not transitioning, add input
		case Enums::GameState::Game:
		case Enums::GameState::Menu:
		{
			// Add values to container
			m_newInput.m_inputIndex = m_reusableIterator_3;
			m_newInput.m_inputPressState = mpp_inputPressStates[m_reusableIterator_2][m_reusableIterator_3];

			// Add input to queue
			mr_sharedInput.m_inputQueueMutex.lock();
			mr_sharedInput.m_inputQueue[m_reusableIterator_2].push(m_newInput);
			mr_sharedInput.m_inputQueueMutex.unlock();
		}
		break;
		}
		mr_sharedGame.m_gameStateMutex.unlock();
	}

	// If a player is trying to pause the game
	else
	{
		mr_sharedInput.m_inputQueueMutex.lock();

		// Clear each queue
		for (m_reusableIterator_4 = Consts::NO_VALUE; m_reusableIterator_4 < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM; m_reusableIterator_4++)
		{
			while (mr_sharedInput.m_inputQueue[m_reusableIterator_4].empty() == false)
			{
				mr_sharedInput.m_inputQueue[m_reusableIterator_4].pop();
			}
		}

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::PauseGame;
		mr_sharedGame.m_gameStateMutex.unlock();
		mr_sharedInput.m_inputQueueMutex.unlock();
	}
}
#pragma endregion

#pragma region Destruction
InputManager::~InputManager()
{
	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM; m_reusableIterator_1++)
	{
		delete[] mpp_inputPressStates[m_reusableIterator_1];
		delete[] mpp_deadFramesTargetFrames[m_reusableIterator_1];
	}

	delete[] mpp_inputPressStates;
	delete[] mpp_deadFramesTargetFrames;
}
#pragma endregion