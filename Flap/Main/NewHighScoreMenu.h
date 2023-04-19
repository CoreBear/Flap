#ifndef NEW_HIGH_SCORE_MENU_H
#define NEW_HIGH_SCORE_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DList.h"
#include "DynamicTextMenu.h"
#include "SharedGame.h"
#include "TextLine.h"

class NewHighScoreMenu final : public DynamicTextMenu
{
public:
	// Initialization
	NewHighScoreMenu() : DynamicTextMenu(2)	// This value must match the number of text lines below
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("New High Score", Consts::OFF_BY_ONE),
			//new TextLine("Press left (arrow/A) and right (arrow/D), while on the initial/score line, to choose initial cell. Type the character you'd like to enter.", 9),
			//new TextLine("Please enter your initials: ", 10),
			new TextLine("To Results", 15)
		};
	}
	NewHighScoreMenu(const NewHighScoreMenu&) = delete;
	NewHighScoreMenu& operator=(const NewHighScoreMenu&) = delete;
	void Initialize() override
	{
		ClearCells();

		m_initialIndex = Consts::NO_VALUE;
		
		memset(sp_sharedGame->m_initials, Consts::EMPTY_SPACE_CHAR, sp_sharedGame->MAX_NUMBER_OF_INITIALS);
		
		// Generate instruction line
		{
			mp_newString = new char[UCHAR_MAX];
			strcpy(mp_newString, "Press left arrow and right arrow, to choose a cell. Type the character you'd like to enter. Press enter to containue.");

			mp_walker = mp_newString;

			m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

			while (*mp_walker != '\0')
			{
				mp_newBufferCell = new BufferCell;
				mp_newBufferCell->m_character = *mp_walker;
				mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
				mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
				mp_newBufferCell->m_position.m_y = 9;
				m_cells.PushBack(mp_newBufferCell);

				++mp_walker;
			}

			delete[] mp_newString;
		}

		// Generate initial/score line
		{
			mp_newString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
			strcpy(mp_newString, "Please enter your initials: ___ ");

			const char* intString = Tools::IntToString(sp_sharedGame->m_largestSnakeLengthUponDeath);
			strcat(mp_newString, intString);
			delete[] intString;

			mp_walker = mp_newString;

			m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

			while (*mp_walker != '\0')
			{
				mp_newBufferCell = new BufferCell;
				mp_newBufferCell->m_character = *mp_walker;
				mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
				mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
				mp_newBufferCell->m_position.m_y = 10;
				m_cells.PushBack(mp_newBufferCell);

				++mp_walker;
			}

			for (m_updateCellIterator = m_cells.Begin(); m_updateCellIterator != m_cells.End(); ++m_updateCellIterator)
			{
				if ((*m_updateCellIterator)->m_character == '_')
				{
					(*m_updateCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
					break;
				}
			}

			delete[] mp_newString;
		}
	}

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::SaveHighScores;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
	void InputCharacter(int _inputIndexOrKeyCode) override 
	{
		(*m_updateCellIterator)->m_character = static_cast<char>(_inputIndexOrKeyCode);
		sp_sharedGame->m_initials[m_initialIndex] = static_cast<char>(_inputIndexOrKeyCode);
	}
	void NextOption() override
	{
		// If index can move right, move it right
		if (m_initialIndex < sp_sharedGame->MAX_NUMBER_OF_INITIALS - Consts::OFF_BY_ONE)
		{
			++m_initialIndex;

			(*m_updateCellIterator)->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
			++m_updateCellIterator;
			(*m_updateCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
		}
	}
	void PreviousOption() override
	{
		// If index can move left, move it left
		if (m_initialIndex > 0)
		{
			--m_initialIndex;

			(*m_updateCellIterator)->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
			--m_updateCellIterator;
			(*m_updateCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
		}
	}

private:
	// Member Variables
	int m_initialIndex;
	DList<BufferCell*>::Iterator m_updateCellIterator;
};

#endif NEW_HIGH_SCORE_MENU_H