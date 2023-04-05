#ifndef Network_MENU_H
#define Network_MENU_H

#include "MenuBase.h"
#include "Structure.h"

class NetworkMenu final : public MenuBase
{
public:
	// Initialization
	NetworkMenu() : MenuBase(1)	// This value must match the number of text lines below
	{
		mp_textLines = new Structure::TextLine * [m_numberOfTextLines]
		{
			new Structure::TextLine("Network", 5, 5)
		};
	}
	NetworkMenu(const NetworkMenu&) = delete;
	NetworkMenu& operator=(const NetworkMenu&) = delete;
};

#endif Network_MENU_H