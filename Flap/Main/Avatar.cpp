#pragma region Includes
#include "Avatar.h"

#include "SharedMemory.h"
#pragma endregion

#pragma region Initialization
Avatar::Avatar(SharedMemory& _sharedMemory)
{
	static int assignInputReceiverIndex = Consts::NO_VALUE;
	mp_inputQueueMutex = &_sharedMemory.GetInputQueueMutexRef(assignInputReceiverIndex);
	mp_inputQueue = &_sharedMemory.GetInputQueueRef(assignInputReceiverIndex++);
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