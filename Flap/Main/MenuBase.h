#ifndef MENU_BASE_H
#define MENU_BASE_H

#include "Consts.h"
#include "DList.h"

class BufferCell;
class SharedGame;
class TextLine; 

class MenuBase 
{
public:
	// Static Initialization
	static void AssignSharedGame(SharedGame& _sharedGame) { sp_sharedGame = &_sharedGame; }

	// Member Variables
	DList<BufferCell*> m_cells;
	int m_currNumOfTextLines;
	int m_totNumOfTextLines;
	TextLine** mp_textLines;

	// Initialzation
	inline virtual void Initialize() { return; }
	MenuBase(const MenuBase&) = delete;
	MenuBase& operator=(const MenuBase&) = delete;

	// Updates
	inline virtual void FixedUpdate() { return; }

	// Functionality
	inline int GetCurrentButtonNumber() const { return m_currentButtonNumber; }
	inline int InputAccept() { return InputAcceptHandling(); }
	inline virtual void InputCharacter(int _inputIndexOrKeyCode) { return; }
	void InputDown();
	void InputLeft();
	void InputRight();
	void InputUp();
	inline void ResetButtonNumber() { m_currentButtonNumber = Consts::OFF_BY_ONE; }	// NOTE: Doesn't go to index 0, because index 0 is the menu name's index

	// Destruction
	virtual ~MenuBase();

protected:
	// Static Variables
	static SharedGame* sp_sharedGame;

	// Member Variables
	int m_currentButtonNumber;	// NOTE/WARNING: Using number, not index, because the title is accessible text

	// Initialization
	MenuBase(int _numberOfTextLines) :
		m_currNumOfTextLines(_numberOfTextLines),
		m_totNumOfTextLines(_numberOfTextLines),
		m_currentButtonNumber(Consts::OFF_BY_ONE) 
	{
		return;
	}

	// Functionality
	// NOTE: This function should never actually be called
	inline virtual void ClearCells() { m_cells.Clear(); }
	inline virtual int InputAcceptHandling() { return -1; };
	inline virtual void NextOption() { return; }
	inline virtual void PreviousOption() { return; }
	inline void UpdateNumberOfTextLines(int _numberOfTextLines) { m_currNumOfTextLines = _numberOfTextLines; }
};

#endif MENU_BASE_H