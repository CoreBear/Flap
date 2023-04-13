#ifndef SINGLE_PLAYER_MENU_H
#define SINGLE_PLAYER_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class SinglePlayerMenu final : public MenuBase
{
public:
	// Initialization
	SinglePlayerMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Single Player", Consts::OFF_BY_ONE),			// Menu Title
			new TextLine("Play", 5),
			new TextLine("Return", 10)
		};
	}
	SinglePlayerMenu(const SinglePlayerMenu&) = delete;
	SinglePlayerMenu& operator=(const SinglePlayerMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::PlayGame;
		case 2:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif SINGLE_PLAYER_MENU_H