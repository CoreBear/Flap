#ifndef RESULTS_MULTI_MENU_H
#define RESULTS_MULTI_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DynamicTextMenu.h"
#include "Structure.h"
#include "TextLine.h"
#include "Tools.h"

class ResultsMultiMenu final : public DynamicTextMenu
{
public:
	// Initialization
	void Initialize() override
	{
		ClearCells();

		mp_largestSnakeLenghtString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
		strcpy(mp_largestSnakeLenghtString, "Largest Snake: ");

		const char* intString = Tools::IntToString(sp_sharedGame->m_largestSnakeLengthUponDeath);
		strcat(mp_largestSnakeLenghtString, intString);
		delete[] intString;
		
		mp_walker = mp_largestSnakeLenghtString;

		m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_largestSnakeLenghtString);

		while (*mp_walker != '\0')
		{
			constexpr int ROW = 5;
			
			mp_newBufferCell = new BufferCell;
			mp_newBufferCell->m_character = *mp_walker;
			mp_newBufferCell->m_colorBFGround = sp_sharedGame->m_largestSnakeColor | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::Black)];
			mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
			mp_newBufferCell->m_position.m_y = ROW;
			m_cells.PushBack(mp_newBufferCell);

			++mp_walker;
		}

		delete[] mp_largestSnakeLenghtString;
	}
	ResultsMultiMenu() : 
		DynamicTextMenu(2),	// This value must match the number of text lines below
		mp_largestSnakeLenghtString(nullptr)
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Results", Consts::OFF_BY_ONE),   // Menu Title
			//new TextLine("Largest Snake:", 5),
			new TextLine("To Main", 10)
		};
	}
	ResultsMultiMenu(const ResultsMultiMenu&) = delete;
	ResultsMultiMenu& operator=(const ResultsMultiMenu&) = delete;

	// Destruction
	inline ~ResultsMultiMenu() { ClearCells(); }

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::Main;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};

private:
	// Member Variables
	BufferCell* mp_newBufferCell;
	char* mp_largestSnakeLenghtString;
	const char* mp_walker;
	int m_columnPositionOffset;
};

#endif RESULTS_MULTI_MENU_H