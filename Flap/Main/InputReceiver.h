#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#include "Enums.h"
#include "Structure.h"

#include <mutex>
#include <queue>

class SharedMemory;

class InputReceiver
{
public:
	// Static Initialization
	static void AssignSharedMemory(SharedMemory& _sharedMemory) { sp_sharedMemory = &_sharedMemory; }

protected:
	// Static Variables
	static SharedMemory* sp_sharedMemory;
	
	// Member Variables
	Structure::Input m_currentInput;
	std::mutex* mp_inputQueueMutex;
	std::queue<Structure::Input>* mp_inputQueue;

	// Functionality
	void HandleInput();
	virtual void InputDown(Enums::InputPressState _inputPressState) { return; }
	virtual void InputLeft(Enums::InputPressState _inputPressState) { return; }
	virtual void InputRight(Enums::InputPressState _inputPressState) { return; }
	virtual void InputUp(Enums::InputPressState _inputPressState) { return; }
};

#endif INPUT_RECEIVER_H