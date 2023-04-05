#ifndef Results_MENU_H
#define Results_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class ResultsMenu final : public MenuBase
{
public:
	// Initialization
	ResultsMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Results", 5, 5)
		};
	}
	ResultsMenu(const ResultsMenu&) = delete;
	ResultsMenu& operator=(const ResultsMenu&) = delete;
};

#endif Results_MENU_H