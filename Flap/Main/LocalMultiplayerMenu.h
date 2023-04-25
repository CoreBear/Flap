#ifndef LOCAL_MULTIPLAYER_MENU_H
#define LOCAL_MULTIPLAYER_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class LocalMultiplayerMenu final : public MenuBase
{
public:
	// Initialization
	LocalMultiplayerMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Local Multiplayer", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Play Game", 5),
			new TextLine("Return", 10)
		};
	}
	LocalMultiplayerMenu(const LocalMultiplayerMenu&) = delete;
	LocalMultiplayerMenu& operator=(const LocalMultiplayerMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::PlayGameLocal;
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

#endif LOCAL_MULTIPLAYER_MENU_H