#pragma region Includes
#include "MenuBase.h"

#include "Consts.h"
#include "SharedGame.h"
#include "TextLine.h"
#pragma endregion

#pragma region Static Initialization
SharedGame* MenuBase::sp_sharedGame = nullptr;
#pragma endregion

#pragma region Public Functionality
void MenuBase::InputDown()
{
	// If user has made it beyond the buttom, loop back to the top button
	if (++m_currentButtonNumber == m_currNumOfTextLines)
	{
		m_currentButtonNumber = Consts::OFF_BY_ONE;
	}
}
void MenuBase::InputLeft()
{
	PreviousOption();
}
void MenuBase::InputRight()
{
	NextOption();
}
void MenuBase::InputUp()
{
	// If user has made it beyond the top, loop back to the bottom button
	if (--m_currentButtonNumber == Consts::NO_VALUE)
	{
		m_currentButtonNumber = m_currNumOfTextLines - Consts::OFF_BY_ONE;
	}
}
#pragma endregion

#pragma region Destruction
MenuBase::~MenuBase()
{
	for (int i = Consts::NO_VALUE; i < m_totNumOfTextLines; i++)
	{
		delete mp_textLines[i];
	}

	delete[] mp_textLines;
}
#pragma endregion