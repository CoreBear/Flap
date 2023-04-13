#ifndef WELCOME_MENU_H
#define WELCOME_MENU_H

#include "Consts.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"

class WelcomeMenu final : public MenuBase
{
public:
	// Initialization
	WelcomeMenu() : MenuBase(2)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Welcome", Consts::OFF_BY_ONE),			// Menu Title
			new TextLine("Press ENTER to continue.", 20)
		};
	}
	WelcomeMenu(const WelcomeMenu&) = delete;
	WelcomeMenu& operator=(const WelcomeMenu&) = delete;

protected:
	// Functionality
	inline virtual int InputAcceptHandling() override
	{
		return Enums::MenuReturn::Main;
	};
};

#endif WELCOME_MENU_H