#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Enums.h"
#include "GameThreadBase.h"
#include "Structure.h"

#include <mutex>
#include <queue>
#include <Windows.h>

class SharedGame;
class SharedInput;

class InputManager final : public GameThreadBase
{
public:
	// Initialization
	InputManager(SharedGame& _sharedGame, SharedInput& _sharedInput);
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
		
	// Updates
	void Update() override;

	// Destruction
	~InputManager() override;

private:
	// Static Variables
	static constexpr unsigned int NUMBER_OF_DEAD_FRAMES = 15;		// Arbitrary value, represents click-to-hold number of frames
	
	// Member Variables
	const DWORD BUFFER_LENGTH;
	const HANDLE INPUT_WINDOW_HANDLE;
	Structure::Input m_newInput;
	INPUT_RECORD m_inputRecords[Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM * Consts::NUMBER_OF_INPUTS];
	Enums::InputPressState** mpp_inputPressStates;
	int m_numberOfEventsRead;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	int m_reusableIterator_3;
	int m_reusableIterator_4;
	SharedGame& mr_sharedGame;
	SharedInput& mr_sharedInput;
	unsigned int** mpp_deadFramesTargetFrames;

	// Functionality
	void ClearQueuesAndUpdateGameActivity(int _gameActivityIndex);
	void ReadAndEnqueueInput(const KEY_EVENT_RECORD& _inputInfo);
};

#endif INPUT_MANAGER_H