#ifndef OVERLAY_MANAGER_H
#define OVERLAY_MANAGER_H

#include "Manager.h"

class OverlayManager final : public Manager
{
public:
	// Initialzation
	OverlayManager() = default;
	OverlayManager(const OverlayManager&) = delete;
	OverlayManager& operator=(const OverlayManager&) = delete;

	// Updates
	void FixedUpdate() { return; }
};
#endif OVERLAY_MANAGER_H