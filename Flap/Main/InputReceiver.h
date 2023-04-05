#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#include "Enums.h"
#include "Structure.h"

#include <mutex>
#include <queue>

class SharedInput;

class InputReceiver
{
public:
	// Static Initialization
	inline static void AssignSharedInput(SharedInput& _sharedInput) { sp_sharedInput = &_sharedInput; }

	// Initialization
	InputReceiver(const InputReceiver&) = delete;
	InputReceiver& operator=(const InputReceiver&) = delete;

protected:
	// Static Variables
	static SharedInput* sp_sharedInput;

	// Member Variables
	Structure::Input m_currentInput;
	int m_playerIndex;

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