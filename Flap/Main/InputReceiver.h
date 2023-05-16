#ifndef INPUT_RECEIVER_H
#define INPUT_RECEIVER_H

#include "Enums.h"
#include "Structure.h"

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
	int m_readIndex;

	// Initialization
	InputReceiver() = default;

	// Functionality
	void HandleInput();
	inline virtual void InputAccept(Enums::InputPressState _inputPressState) { return; }
	inline virtual void InputCharacter(int _inputIndexOrKeyCode) { return; }
	inline virtual void InputDown(Enums::InputPressState _inputPressState) { return; }
	inline virtual void InputLeft(Enums::InputPressState _inputPressState) { return; }
	inline virtual void InputPause(Enums::InputPressState _inputPressState) { return; }
	inline virtual void InputRight(Enums::InputPressState _inputPressState) { return; }
	inline virtual void InputUp(Enums::InputPressState _inputPressState) { return; }
	inline void UpdateReadIndex(int _readIndex) { m_readIndex = _readIndex; }
};

#endif INPUT_RECEIVER_H