#ifndef RESULTS_MULTI_MENU_H
#define RESULTS_MULTI_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DList.h"
#include "MenuBase.h"
#include "Structure.h"
#include "TextLine.h"
#include "Tools.h"

class ResultsMultiMenu final : public MenuBase
{
public:
	// Initialization
	void Initialize() override
	{
		m_cells.Clear();
		delete[] mp_largestSnakeLenghtString;

		mp_largestSnakeLenghtString = Tools::IntToString(sp_sharedGame->m_largestSnakeLengthUponDeath);
		
		mp_walker = mp_largestSnakeLenghtString;
		m_columnPositionOffset = STARTING_OFFSET;

		while (*mp_walker != '\0')
		{
			mp_newBufferCell = new BufferCell;
			mp_newBufferCell->m_character = *mp_walker;
			mp_newBufferCell->m_colorBFGround = sp_sharedGame->m_largestSnakeColor | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::Black)];
			mp_newBufferCell->m_position.m_x = mp_textLines[1]->m_textLineEndColum + m_columnPositionOffset++;
			mp_newBufferCell->m_position.m_y = mp_textLines[1]->m_position.m_y;
			m_cells.PushBack(mp_newBufferCell);

			++mp_walker;
		}
	}
	ResultsMultiMenu() : 
		MenuBase(3),	// This value must match the number of text lines below
		mp_largestSnakeLenghtString(nullptr)
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Results", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Largest Snake:", 5),
			new TextLine("To Main", 10)
		};
	}
	ResultsMultiMenu(const ResultsMultiMenu&) = delete;
	ResultsMultiMenu& operator=(const ResultsMultiMenu&) = delete;

	// Destruction
	~ResultsMultiMenu() 
	{
		for (DList<BufferCell*>::Iterator it = m_cells.Begin(); it != m_cells.End(); ++it)
		{
			delete (*it);
		}

		delete[] mp_largestSnakeLenghtString;
	}

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 2:
			return Enums::MenuReturn::Main;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};

private:
	// Static Variables
	static constexpr int STARTING_OFFSET = 2;	// Arbitrary 2

	// Member Variables
	BufferCell* mp_newBufferCell;
	const char* mp_largestSnakeLenghtString;
	const char* mp_walker;
	int m_columnPositionOffset;
};

#endif RESULTS_MULTI_MENU_H