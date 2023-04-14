#ifndef RESULTS_MENU_H
#define RESULTS_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class ResultsMenu final : public MenuBase
{
public:
	// Initialization
	ResultsMenu() : MenuBase(2)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Results", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("To Main", 5)
		};
	}
	ResultsMenu(const ResultsMenu&) = delete;
	ResultsMenu& operator=(const ResultsMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::Main;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif RESULTS_MENU_H