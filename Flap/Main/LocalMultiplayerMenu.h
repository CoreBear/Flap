#ifndef LOCAL_MULTIPLAYER_MENU_H
#define LOCAL_MULTIPLAYER_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class LocalMultiplayerMenu final : public MenuBase
{
public:
	// Initialization
	LocalMultiplayerMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Local Multiplayer", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Play Game", 5),
			new Structure::TextLine("Return", 10)
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
			return Enums::MenuReturn::PlayGame;
		case 2:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	};
};

#endif LOCAL_MULTIPLAYER_MENU_H