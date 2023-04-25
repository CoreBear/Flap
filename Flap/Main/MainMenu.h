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
	MainMenu() : MenuBase(6)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Main", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Solo", 5),
			new TextLine("Multiplayer", 10),
			new TextLine("High Scores (Single Player)", 15),
			new TextLine("Options", 20),
			new TextLine("Exit", 25)
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
			return Enums::MenuReturn::ShowHighScores;
		case 4:
			return Enums::MenuReturn::Options;
		case 5:
			return Enums::MenuReturn::ExitApp;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif MAIN_MENU_H