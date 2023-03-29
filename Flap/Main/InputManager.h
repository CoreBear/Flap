#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Enums.h"
#include "Manager.h"

#include <Windows.h>

class AtomicMemory;

class InputManager : public Manager
{
public:
	// Initialization
	InputManager(AtomicMemory& _atomicMemory, HANDLE& _windowHandle);
		
	// Updates
	void Update() override;

private:
	Enums::ButtonPressState& m_spaceBarPressState;
	HANDLE m_windowHandle;
	DWORD m_numberOfEventsRead;
	INPUT_RECORD m_inputEvent;
	unsigned int m_deadFrameTargetFrame;
};

#endif INPUT_MANAGER_H