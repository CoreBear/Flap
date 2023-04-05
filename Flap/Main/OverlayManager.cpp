#pragma region Includes
#include "OverlayManager.h"

#include "SharedCollisionRender.h"
#include "SharedInput.h"
#include "Structure.h"
#pragma endregion

#pragma region Initialization
OverlayManager::OverlayManager(SharedCollisionRender& _sharedCollisionRender)
{
	return; // The other renderer, not the one on the other thread..
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