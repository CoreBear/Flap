#ifndef Multiplayer_MENU_H
#define Multiplayer_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class MultiplayerMenu final : public MenuBase
{
public:
	// Initialization
	MultiplayerMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Multiplayer", 5, 5)
		};
	}
	MultiplayerMenu(const MultiplayerMenu&) = delete;
	MultiplayerMenu& operator=(const MultiplayerMenu&) = delete;
};

#endif Multiplayer_MENU_H