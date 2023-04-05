#ifndef Options_MENU_H
#define Options_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class OptionsMenu final : public MenuBase
{
public:
	// Initialization
	OptionsMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Options", 5, 5)
		};
	}
	OptionsMenu(const OptionsMenu&) = delete;
	OptionsMenu& operator=(const OptionsMenu&) = delete;
};

#endif Options_MENU_H