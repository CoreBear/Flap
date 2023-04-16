#ifndef RESULTS_SINGLE_MENU_H
#define RESULTS_SINGLE_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class ResultsSingleMenu final : public MenuBase
{
public:
	// Initialization
	ResultsSingleMenu() : MenuBase(2)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Results", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("To Main", 5)
		};
	}
	ResultsSingleMenu(const ResultsSingleMenu&) = delete;
	ResultsSingleMenu& operator=(const ResultsSingleMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::SaveHighScores;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif RESULTS_SINGLE_MENU_H