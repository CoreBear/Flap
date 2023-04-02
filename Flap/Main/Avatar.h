#ifndef AVATAR_H
#define AVATAR_H

#include "InputReceiver.h"
#include "Snake.h"

class SharedMemory;

class Avatar final : public InputReceiver, public Snake
{
public:
	// Initialization
	Avatar(SharedMemory& _sharedMemory);
	Avatar(const Avatar&) = delete;
	Avatar& operator=(const Avatar&) = delete;

	// Updates
	void FixedUpdate() override;

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;
};

#endif AVATAR_H