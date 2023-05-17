#pragma region Includes
#include "Avatar.h"
#pragma endregion

#pragma region Initialization
Avatar::Avatar()
{
	static int playerIndex = Consts::NO_VALUE;
	m_readIndex = playerIndex++;
}
#pragma endregion

#pragma region Updates
void Avatar::FixedUpdate()
{
	InputReceiver::HandleInput();

	Snake::FixedUpdate();
}
#pragma endregion

#pragma region Private Functionality
void Avatar::InputDown(Enums::InputPressState _inputPressState)
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
		Snake::InputDown();
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void Avatar::InputLeft(Enums::InputPressState _inputPressState)
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
		Snake::InputLeft();
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void Avatar::InputRight(Enums::InputPressState _inputPressState)
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
		Snake::InputRight();
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
void Avatar::InputUp(Enums::InputPressState _inputPressState)
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
		Snake::InputUp();
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
#pragma endregion