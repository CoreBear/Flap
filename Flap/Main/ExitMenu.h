#ifndef Exit_MENU_H
#define Exit_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class ExitMenu final : public MenuBase
{
public:
	// Initialization
	ExitMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Exit", 5, 5)
		};
	}
	ExitMenu(const ExitMenu&) = delete;
	ExitMenu& operator=(const ExitMenu&) = delete;
};

#endif Exit_MENU_H