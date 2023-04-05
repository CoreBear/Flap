#pragma region Includes
#include "MenuBase.h"

#include "Structure.h"
#pragma endregion

#pragma region Destruction
MenuBase::~MenuBase()
{
	for (int i = 0; i < m_numberOfTextLines; i++)
	{
		delete mp_textLines[i];
	}

	delete[] mp_textLines;
}
#pragma endregion