#ifndef OVERLAY_MANAGER_H
#define OVERLAY_MANAGER_H

#include "InputReceiver.h"

class SharedCollisionRender;
class SharedInput;

class OverlayManager final : public InputReceiver
{
public:
	// Initialzation
	OverlayManager(SharedCollisionRender& _sharedCollisionRender);
	OverlayManager(const OverlayManager&) = delete;
	OverlayManager& operator=(const OverlayManager&) = delete;

	// Updates
	void FixedUpdate();
	void Update() { return; }

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;
};
#endif OVERLAY_MANAGER_H