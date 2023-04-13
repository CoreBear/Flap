#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class MainMenu final : public MenuBase
{
public:
	// Initialization
	MainMenu() : MenuBase(5)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Main", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Solo", 5),
			new TextLine("Multiplayer", 10),
			new TextLine("Options", 15),
			new TextLine("Exit", 20)
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

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif MAIN_MENU_H