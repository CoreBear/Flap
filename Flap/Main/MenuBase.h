#ifndef MENU_BASE_H
#define MENU_BASE_H

#include "Consts.h"

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

	// Functionality
	inline int GetCurrentButtonNumber() const { return m_currentButtonNumber; }
	inline int InputAccept() { return InputAcceptHandling(); }
	void InputDown();
	void InputLeft();
	void InputRight();
	void InputUp();
	inline void ResetButtonNumber() { m_currentButtonNumber = Consts::OFF_BY_ONE; }	// NOTE: Doesn't go to index 0, because index 0 is the menu name's index

	// Destruction
	~MenuBase();

protected:
	// Member Variables
	int m_currentButtonNumber;	// NOTE/WARNING: Using number, not index, because the title is accessible text

	// Initialization
	MenuBase(int _numberOfTextLines) : m_numberOfTextLines(_numberOfTextLines), m_currentButtonNumber(Consts::OFF_BY_ONE) { return; }

	// Functionality
	// NOTE: This function should never actually be called
	inline virtual int InputAcceptHandling() { return -1; };
};

#endif MENU_BASE_H