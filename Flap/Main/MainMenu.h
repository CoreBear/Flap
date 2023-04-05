#ifndef Main_MENU_H
#define Main_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class MainMenu final : public MenuBase
{
public:
	// Initialization
	MainMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Main", 5, 5)
		};
	}
	MainMenu(const MainMenu&) = delete;
	MainMenu& operator=(const MainMenu&) = delete;
};

#endif Main_MENU_H