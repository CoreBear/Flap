#pragma region Includes
#include "Tools.h"

#include "SharedRender.h"

#include <cstring>
#pragma endregion

#pragma region Static Initialization
SharedRender* Tools::sp_sharedRender = nullptr;
#pragma endregion

#pragma region Functionality
int Tools::CenterText_ReturnStartColumn(const char* _text)
{
	return (sp_sharedRender->m_bufferHW.X / 2) - strlen(_text) / 2;
}
#pragma endregion