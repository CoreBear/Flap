#pragma region Includes
#include "SharedInput.h"

#include "InputReceiver.h"
#pragma endregion

#pragma region Initialization

SharedInput::SharedInput()
{
	InputReceiver::AssignSharedInput(*this); 
}
#pragma endregion