#ifndef EXIT_MENU_H
#define EXIT_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class ExitMenu final : public MenuBase
{
public:
	// Initialization
	ExitMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Exit", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Return", 5),
			new TextLine("Quit Game", 10)
		};
	}
	ExitMenu(const ExitMenu&) = delete;
	ExitMenu& operator=(const ExitMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::Return;
		case 2:
			return Enums::MenuReturn::ExitToMain;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif EXIT_MENU_H