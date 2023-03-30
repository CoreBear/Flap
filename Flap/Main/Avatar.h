#ifndef AVATAR_H
#define AVATAR_H

#include "InputReceiver.h"
#include "Snake.h"

class Avatar final : public InputReceiver, public Snake
{
public:
	// Initialization
	Avatar();

	// Updates
	void Update() override final;

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override final;
	void InputLeft(Enums::InputPressState _inputPressState) override final;
	void InputRight(Enums::InputPressState _inputPressState) override final;
	void InputUp(Enums::InputPressState _inputPressState) override final;
};

#endif AVATAR_H