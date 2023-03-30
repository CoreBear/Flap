#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Enums.h"
#include "Manager.h"
#include "Structure.h"

#include <queue>
#include <Windows.h>

class SharedMemory;

class InputManager final : public Manager
{
public:
	// Initialization
	InputManager(HANDLE& _windowHandle, SharedMemory& _sharedMemory);
		
	// Updates
	void Update() override;

	// Destruction
	~InputManager() override;

private:
	// Member Variables
	DWORD m_bufferLength;
	HANDLE m_windowHandle;
	Structure::Input m_newInput;
	INPUT_RECORD m_inputRecords[UCHAR_MAX];
	Enums::InputPressState* mp_inputPressStates;
	int m_numberOfEventsRead;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	std::queue<Structure::Input>& mr_inputQueue;
	unsigned int* mp_deadFramesTargetFrames;

	// Functionality
	void ReadAndEnqueueInput(const KEY_EVENT_RECORD& _inputInfo);
};

#endif INPUT_MANAGER_H