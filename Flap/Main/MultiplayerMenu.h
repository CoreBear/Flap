#ifndef MULTIPLAYER_MENU_H
#define MULTIPLAYER_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class MultiplayerMenu final : public MenuBase
{
public:
	// Initialization
	MultiplayerMenu() : MenuBase(4)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Multiplayer", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Local", 5),
			new TextLine("Networked", 10),
			new TextLine("Return", 15),
		};
	}
	MultiplayerMenu(const MultiplayerMenu&) = delete;
	MultiplayerMenu& operator=(const MultiplayerMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::LocalMultiplayer;
		case 2:
			return Enums::MenuReturn::Network;
		case 3:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif MULTIPLAYER_MENU_H