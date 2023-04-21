#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class PauseMenu final : public MenuBase
{
public:
	// Initialization
	PauseMenu() : MenuBase(4)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Pause", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Resume", 5),
			new TextLine("Options", 10),
			new TextLine("Exit", 15)
		};
	}
	PauseMenu(const PauseMenu&) = delete;
	PauseMenu& operator=(const PauseMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::ResumeGame;
		case 2:
			return Enums::MenuReturn::Options;
		case 3:
			return Enums::MenuReturn::Exit;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
};

#endif PAUSE_MENU_H