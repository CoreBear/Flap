#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#include "Enums.h"
#include "Structure.h"

#include <mutex>
#include <queue>

class InputReceiver
{
public:
	// Initialization
	InputReceiver(const InputReceiver&) = delete;
	InputReceiver& operator=(const InputReceiver&) = delete;

protected:
	// Member Variables
	Structure::Input m_currentInput;
	std::mutex* mp_inputQueueMutex;					// Has to be a pointer, because they cannot be initialized via initializer list
	std::queue<Structure::Input>* mp_inputQueue;	// Has to be a pointer, because they cannot be initialized via initializer list

	// Initialization
	InputReceiver() = default;

	// Functionality
	void HandleInput();
	virtual void InputDown(Enums::InputPressState _inputPressState) { return; }
	virtual void InputLeft(Enums::InputPressState _inputPressState) { return; }
	virtual void InputRight(Enums::InputPressState _inputPressState) { return; }
	virtual void InputUp(Enums::InputPressState _inputPressState) { return; }
};

#endif INPUT_RECEIVER_H