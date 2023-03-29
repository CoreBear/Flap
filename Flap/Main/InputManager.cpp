#pragma region Includes
#include "InputManager.h"

#include "AtomicMemory.h"
#include "Consts.h"
#include "SceneManager.h"
#pragma endregion

#pragma region Initialization
InputManager::InputManager(AtomicMemory& _atomicMemory, HANDLE& _windowHandle) :
	m_spaceBarPressState(_atomicMemory.GetButtonPressStateRef()),
	m_windowHandle(GetStdHandle(STD_INPUT_HANDLE)),
	m_numberOfEventsRead(Consts::NO_VALUE),
	m_deadFrameTargetFrame(Consts::NO_VALUE)
{
	m_spaceBarPressState = Enums::ButtonPressState::Released;
}
#pragma endregion

#pragma region Updates
void InputManager::Update() 
{
	if (GetNumberOfConsoleInputEvents(m_windowHandle, &m_numberOfEventsRead))
	{
		ReadConsoleInput(m_windowHandle, &m_inputEvent, 10, &m_numberOfEventsRead);

		if (m_inputEvent.EventType == KEY_EVENT)
		{
			if (m_inputEvent.Event.KeyEvent.wVirtualKeyCode == VK_SPACE)
			{
				if (m_inputEvent.Event.KeyEvent.bKeyDown)
				{
					switch (m_spaceBarPressState)
					{
						// Do nothing
					//case Enums::ButtonPressState::Held:

					case Enums::ButtonPressState::Click:
					case Enums::ButtonPressState::Released:
					{
						m_spaceBarPressState = Enums::ButtonPressState::PressedThisFrame;
					}
					break;
					case Enums::ButtonPressState::Dead:
					{
        				if (SceneManager::s_simFrameCount >= m_deadFrameTargetFrame)
						{
							m_spaceBarPressState = Enums::ButtonPressState::Held;
						}
					}
					break;
					case Enums::ButtonPressState::PressedThisFrame:
					{
						m_spaceBarPressState = Enums::ButtonPressState::Dead;

						// Arbitrary value, represents click-to-hold number of frames
						const unsigned int NUMBER_OF_DEAD_FRAMES = 15;
                      	m_deadFrameTargetFrame = SceneManager::s_simFrameCount + NUMBER_OF_DEAD_FRAMES;
					}
					break;
					}
				}
				else
				{
					switch (m_spaceBarPressState)
					{
						// Cannot occur
					//case Enums::ButtonPressState::Click:
					//case Enums::ButtonPressState::Released:

					case Enums::ButtonPressState::Dead:
					case Enums::ButtonPressState::PressedThisFrame:
					{
						m_spaceBarPressState = Enums::ButtonPressState::Click;
					}
					break;
					case Enums::ButtonPressState::Held:
					{
						m_spaceBarPressState = Enums::ButtonPressState::Released;
					}
					break;
					}
				}
			}
		}
	}
}
#pragma endregion