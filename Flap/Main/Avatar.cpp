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
void Avatar::Update()
{
	InputReceiver::HandleInput();
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
		Snake::BeginDown();
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
		Snake::BeginLeft();
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
		Snake::BeginRight();
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
		Snake::BeginUp();
		break;
	case Enums::InputPressState::Released:
		break;
	}
}
#pragma endregion