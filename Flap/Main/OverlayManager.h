#ifndef OVERLAY_MANAGER_H
#define OVERLAY_MANAGER_H

#include "InputReceiver.h"
#include "Manager.h"

class SharedMemory;

class OverlayManager final : public InputReceiver, public Manager
{
public:
	// Initialzation
	OverlayManager(SharedMemory& _sharedMemory);
	OverlayManager(const OverlayManager&) = delete;
	OverlayManager& operator=(const OverlayManager&) = delete;

	// Updates
	void FixedUpdate();

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;
};
#endif OVERLAY_MANAGER_H