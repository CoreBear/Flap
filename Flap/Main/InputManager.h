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
		
	// Updates
	void Update() override;

	// Destruction
	~InputManager() override;

private:
	// Static Variables
	static constexpr unsigned int NUMBER_OF_DEAD_FRAMES = 15;		// Arbitrary value, represents click-to-hold number of frames
	
	// Member Variables
	bool m_inputMatched;
	DWORD m_bufferLength;
	const HANDLE m_inputWindowHandle;
	Structure::Input m_newInput;
	INPUT_RECORD m_inputRecords[Consts::MAX_NUMBER_OF_PLAYERS * Consts::NUMBER_OF_INPUTS];
	Enums::InputPressState** mpp_inputPressStates;
	int m_numberOfEventsRead;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	int m_reusableIterator_3;
	std::mutex* mp_inputQueueMutex;
	std::queue<Structure::Input>* mp_inputQueue;
	unsigned int** mpp_deadFramesTargetFrames;

	// Functionality
	void ReadAndEnqueueInput(const KEY_EVENT_RECORD& _inputInfo);
};

#endif INPUT_MANAGER_H