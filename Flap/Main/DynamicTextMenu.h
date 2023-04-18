#ifndef DYNAMIC_TEXT_MENU_H
#define DYNAMIC_TEXT_MENU_H

#include "DList.h"
#include "MenuBase.h"

class DynamicTextMenu : public MenuBase
{
protected:
	// Initialization
	inline DynamicTextMenu(int _numberOfTextLines) : MenuBase(_numberOfTextLines) { return; }
	DynamicTextMenu(const DynamicTextMenu&) = delete;
	DynamicTextMenu& operator=(const DynamicTextMenu&) = delete;

	// Functionality
	void ClearCells()
	{
		for (m_clearIterator = m_cells.Begin(); m_clearIterator != m_cells.End(); ++m_clearIterator)
		{
			delete (*m_clearIterator);
		}

		m_cells.Clear();
	}

private:
	// Member Variables
	DList<BufferCell*>::Iterator m_clearIterator;
};

#endif DYNAMIC_TEXT_MENU_H