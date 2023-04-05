#ifndef SinglePlayer_MENU_H
#define SinglePlayer_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class SinglePlayerMenu final : public MenuBase
{
public:
	// Initialization
	SinglePlayerMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("SinglePlayer", 5, 5)
		};
	}
	SinglePlayerMenu(const SinglePlayerMenu&) = delete;
	SinglePlayerMenu& operator=(const SinglePlayerMenu&) = delete;
};

#endif SinglePlayer_MENU_H