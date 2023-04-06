#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class OptionsMenu final : public MenuBase
{
public:
	// Initialization
	OptionsMenu() : MenuBase(2)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Options", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Return", 5)
		};
	}
	OptionsMenu(const OptionsMenu&) = delete;
	OptionsMenu& operator=(const OptionsMenu&) = delete;

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	};
};

#endif OPTIONS_MENU_H