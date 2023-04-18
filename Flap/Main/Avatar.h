#ifndef AVATAR_H
#define AVATAR_H

#include "InputReceiver.h"
#include "Snake.h"

class Avatar final : public InputReceiver, public Snake
{
public:
	// Initialization
	Avatar();
	Avatar(const Avatar&) = delete;
	Avatar& operator=(const Avatar&) = delete;
	void Initialize(const Structure::Generic* const _genericContainer) override;

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