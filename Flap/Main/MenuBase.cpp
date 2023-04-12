#pragma region Includes
#include "MenuBase.h"
#pragma endregion

#pragma region Public Functionality
void MenuBase::InputDown()
{
	// If user has made it beyond the buttom, loop back to the top button
	if (++m_currentButtonNumber == m_numberOfTextLines)
	{
		m_currentButtonNumber = Consts::OFF_BY_ONE;
	}
}
void MenuBase::InputLeft()
{

}
void MenuBase::InputRight()
{

}
void MenuBase::InputUp()
{
	// If user has made it beyond the top, loop back to the bottom button
	if (--m_currentButtonNumber == Consts::NO_VALUE)
	{
		m_currentButtonNumber = m_numberOfTextLines - Consts::OFF_BY_ONE;
	}
}
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