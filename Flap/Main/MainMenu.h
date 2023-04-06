#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class MainMenu final : public MenuBase
{
public:
	// Initialization
	MainMenu() : MenuBase(5)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Main", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Solo", 5),
			new Structure::TextLine("Multiplayer", 10),
			new Structure::TextLine("Options", 15),
			new Structure::TextLine("Exit", 20)
		};
	}
	MainMenu(const MainMenu&) = delete;
	MainMenu& operator=(const MainMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::SinglePlayer;
		case 2:
			return Enums::MenuReturn::Multiplayer;
		case 3:
			return Enums::MenuReturn::Options;
		case 4:
			return Enums::MenuReturn::ExitApp;
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	};
};

#endif MAIN_MENU_H