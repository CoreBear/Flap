#ifndef RESULTS_MENU_H
#define RESULTS_MENU_H

#include "Consts.h"
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
			new Structure::TextLine("Results", Consts::OFF_BY_ONE),   // Menu Title
		};
	}
	ResultsMenu(const ResultsMenu&) = delete;
	ResultsMenu& operator=(const ResultsMenu&) = delete;
};

#endif RESULTS_MENU_H