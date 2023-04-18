#pragma region Includes
#include "SharedInput.h"

#include "InputReceiver.h"
#pragma endregion

#pragma region Initialization
SharedInput::SharedInput() : m_inputType(Enums::InputType::Normal)
{
	InputReceiver::AssignSharedInput(*this); 
}
#pragma endregion