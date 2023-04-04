#pragma region Includes
#include "OverlayManager.h"

#include "SharedMemory.h"
#include "Structure.h"
#pragma endregion

#pragma region Initialization
OverlayManager::OverlayManager(SharedMemory& _sharedMemory)
{
	mp_inputQueueMutex = &_sharedMemory.GetInputQueueMutexRef(Consts::NO_VALUE);
	mp_inputQueue = &_sharedMemory.GetInputQueueRef(Consts::NO_VALUE);
}
#pragma endregion

#pragma region Updates
void OverlayManager::FixedUpdate()
{
	InputReceiver::HandleInput();
}
#pragma endregion

#pragma region Private Functionality
void OverlayManager::InputDown(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void OverlayManager::InputLeft(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void OverlayManager::InputRight(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void OverlayManager::InputUp(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Click:
		break;
	case Enums::InputPressState::Dead:
		break;
	case Enums::InputPressState::Held:
		break;
	case Enums::InputPressState::PressedThisFrame:
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
#pragma endregion