#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Enums.h"
#include "Manager.h"
#include "Structure.h"

#include <mutex>
#include <queue>
#include <Windows.h>

class SharedMemory;

class InputManager final : public Manager
{
public:
	// Initialization
	InputManager(SharedMemory& _sharedMemory);
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
	INPUT_RECORD m_inputRecords[Consts::MAX_NUMBER_OF_PLAYERS * Consts::NUMBER_OF_INPUTS];
	Enums::InputPressState** mpp_inputPressStates;
	int m_numberOfEventsRead;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	int m_reusableIterator_3;
	std::mutex* const mp_inputQueueMutex;					// Needs to be a pointer, because it's an array
	std::queue<Structure::Input>* const mp_inputQueue;		// Needs to be a pointer, because it's an array
	unsigned int** mpp_deadFramesTargetFrames;

	// Functionality
	void ReadAndEnqueueInput(const KEY_EVENT_RECORD& _inputInfo);
};

#endif INPUT_MANAGER_H