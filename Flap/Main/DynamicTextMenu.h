#ifndef DYNAMIC_TEXT_MENU_H
#define DYNAMIC_TEXT_MENU_H

#include "DList.h"
#include "MenuBase.h"

class BufferCell;

class DynamicTextMenu : public MenuBase
{
protected:
	// Member Variables
	BufferCell* mp_newBufferCell;
	char* mp_newString;
	const char* mp_walker;
	int m_columnPositionOffset;

	// Initialization
	inline DynamicTextMenu(int _numberOfTextLines) : 
		MenuBase(_numberOfTextLines),
		mp_newString(nullptr)
	{
		return; 
	}
	DynamicTextMenu(const DynamicTextMenu&) = delete;
	DynamicTextMenu& operator=(const DynamicTextMenu&) = delete;

	// Functionality
	void ClearCells() override
	{
		for (m_clearIterator = m_cells.Begin(); m_clearIterator != m_cells.End(); ++m_clearIterator)
		{
			delete (*m_clearIterator);
		}

		MenuBase::ClearCells();
	}

	// Destruction
	~DynamicTextMenu() override
	{
		delete[] mp_newString;

		ClearCells();
	}

private:
	// Member Variables
	DList<BufferCell*>::Iterator m_clearIterator;
};

#endif DYNAMIC_TEXT_MENU_H