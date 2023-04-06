#ifndef NETWORK_MENU_H
#define NETWORK_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"

class NetworkMenu final : public MenuBase
{
public:
	// Initialization
	NetworkMenu() : MenuBase(3)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Network", Consts::OFF_BY_ONE),   // Menu Title
			new Structure::TextLine("Begin Searching", 5),
			new Structure::TextLine("Return", 10)
		};
	}

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::Search;
		case 2:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE/WARNING: Execution shouldn't make it here
		throw std::exception();
		return -Consts::OFF_BY_ONE;
	}
};

#endif NETWORK_MENU_H