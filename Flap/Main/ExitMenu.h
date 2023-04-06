#ifndef EXIT_MENU_H
#define EXIT_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class ExitMenu final : public MenuBase
{
public:
	// Initialization
	ExitMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Exit", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Return", 5),
			new Structure::TextLine("Quit Game", 10)
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
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		case 2:
			return Enums::MenuReturn::ExitToMain;
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	};
};

#endif EXIT_MENU_H