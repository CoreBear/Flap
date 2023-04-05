#ifndef Pause_MENU_H
#define Pause_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class PauseMenu final : public MenuBase
{
public:
	// Initialization
	PauseMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Pause", 5, 5)
		};
	}
	PauseMenu(const PauseMenu&) = delete;
	PauseMenu& operator=(const PauseMenu&) = delete;
};

#endif Pause_MENU_H