#pragma region Includes
#include "Tools.h"

#include "SharedRender.h"
#pragma endregion

#pragma region Static Initialization
SharedRender* Tools::sp_sharedRender = nullptr;
#pragma endregion

#pragma region Functionality
int Tools::CenterText_ReturnStartColumn(int _textLineLength)
{
	return (sp_sharedRender->m_bufferHW.X / 2) - _textLineLength / 2;
}
#pragma endregion