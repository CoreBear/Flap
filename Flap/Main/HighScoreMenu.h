#ifndef HIGH_SCORE_MENU_H
#define HIGH_SCORE_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DynamicTextMenu.h"
#include "SharedGame.h"
#include "Structure.h"
#include "TextLine.h"

class HighScoreMenu final : public DynamicTextMenu
{
public:
	// Initialization
	void Initialize() override
	{
		ClearCells();

		constexpr int ROW_FACTOR = 5;

		for (m_reusableIterator = Consts::NO_VALUE, m_row = Consts::NO_VALUE; m_reusableIterator < sp_sharedGame->MAX_NUMBER_OF_HIGH_SCORES; m_reusableIterator++)
		{
			mp_walker = sp_sharedGame->mpp_highScoreLines[m_reusableIterator];

			m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(sp_sharedGame->mpp_highScoreLines[m_reusableIterator]);
			m_row += ROW_FACTOR;

			while (*mp_walker != '\0')
			{
				mp_newBufferCell = new BufferCell;

				if (*mp_walker != '.')
				{
					mp_newBufferCell->m_character = *mp_walker;
					mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
				}
				else
				{
					mp_newBufferCell->m_character = Consts::EMPTY_SPACE_CHAR;
					mp_newBufferCell->m_colorBFGround = NULL;
				}

				mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
				mp_newBufferCell->m_position.m_y = m_row;

				m_cells.PushBack(mp_newBufferCell);

				++mp_walker;
			}
		}
	}
	HighScoreMenu() : DynamicTextMenu(2)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Results", Consts::OFF_BY_ONE),	// Menu Title
			//new TextLine("", 5),							// High Score Line
			//new TextLine("", 10),							// High Score Line
			//new TextLine("", 15),							// High Score Line
			//new TextLine("", 20),							// High Score Line
			//new TextLine("", 25),							// High Score Line
			//new TextLine("", 30),							// High Score Line
			//new TextLine("", 35),							// High Score Line
			//new TextLine("", 40),							// High Score Line
			//new TextLine("", 45),							// High Score Line
			//new TextLine("", 50),							// High Score Line
			new TextLine("To Main", 55)
		};
	}
	HighScoreMenu(const HighScoreMenu&) = delete;
	HighScoreMenu& operator=(const HighScoreMenu&) = delete;

	// Destruction
	inline ~HighScoreMenu() { ClearCells(); }

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::HighScoreToMain;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};

private:
	// Member Variables
	BufferCell* mp_newBufferCell;
	const char* mp_walker;
	int m_columnPositionOffset;
	int m_reusableIterator;
	int m_row;
};

#endif HIGH_SCORE_MENU_H