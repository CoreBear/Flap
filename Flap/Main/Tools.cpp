#pragma region Includes
#include "Tools.h"

#include "SharedCollisionRender.h"

#include <cstring>
#pragma endregion

#pragma region Static Initialization
SharedCollisionRender* Tools::sp_sharedCollisionRender = nullptr;
#pragma endregion

#pragma region Functionality
int Tools::CenterText_ReturnStartColumn(const char* _text)
{
	return (sp_sharedCollisionRender->SCREEN_BUFFER_CR.X / 2) - strlen(_text) / 2;
}
#pragma endregion