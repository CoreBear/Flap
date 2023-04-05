#ifndef MENU_BASE_H
#define MENU_BASE_H

namespace Structure { struct TextLine; }

class MenuBase
{
public:
	// Member Variables
	const int m_numberOfTextLines;
	Structure::TextLine** mp_textLines;

	// Initialzation
	MenuBase(const MenuBase&) = delete;
	MenuBase& operator=(const MenuBase&) = delete;

	// Destruction
	~MenuBase();

protected:
	// Initialization
	MenuBase(int _numberOfTextLines) : m_numberOfTextLines(_numberOfTextLines) { return; }
};

#endif MENU_BASE_H