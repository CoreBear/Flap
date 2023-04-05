#ifndef Local_MULTIPLAYER_MENU_H
#define Local_MULTIPLAYER_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class LocalMultiplayerMenu final : public MenuBase
{
public:
	// Initialization
	LocalMultiplayerMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Local", 5, 5)
		};
	}
	LocalMultiplayerMenu(const LocalMultiplayerMenu&) = delete;
	LocalMultiplayerMenu& operator=(const LocalMultiplayerMenu&) = delete;
};

#endif Local_MULTIPLAYER_MENU_H