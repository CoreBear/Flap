#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class PauseMenu final : public MenuBase
{
public:
	// Initialization
	PauseMenu() : MenuBase(4)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Pause", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Resume", 5),
			new Structure::TextLine("Options", 10),
			new Structure::TextLine("Exit", 15)
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
			return Enums::MenuReturn::Resume;
		case 2:
			return Enums::MenuReturn::Options;
		case 3:
			return Enums::MenuReturn::Exit;
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	};
};

#endif PAUSE_MENU_H