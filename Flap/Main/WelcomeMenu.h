#ifndef WELCOME_MENU_H
#define WELCOME_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class WelcomeMenu final : public MenuBase
{
public:
	// Initialization
	WelcomeMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Welcome", 5, 5)
		};
	}
	WelcomeMenu(const WelcomeMenu&) = delete;
	WelcomeMenu& operator=(const WelcomeMenu&) = delete;
};

#endif WELCOME_MENU_H