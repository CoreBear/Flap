#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "Enums.h"
#include "MenuBase.h"
#include "SharedGame.h"
#include "Structure.h"
#include "TextLine.h"

class OptionsMenu final : public MenuBase
{
public:
	// Initialization
	// This value must match the number of text lines below
	OptionsMenu() : 
		MenuBase(4),
		mp_playerOneSnakeColorBufferCell(new BufferCell),
		mp_playerTwoSnakeColorBufferCell(new BufferCell)
	{
		mp_textLines = new TextLine * [m_numberOfTextLines]
		{
			new TextLine("Options", Consts::OFF_BY_ONE),   // Menu Title
			new TextLine("Snake 1 Color:", 5),
			new TextLine("Snake 2 Color:", 10),
			new TextLine("Return", 15)
		};

		// NOTE: Arbitrary colors
		sp_sharedGame->SetPlayerSnakeColorIndex(static_cast<int>(Enums::Color::Blue), FIRST_PLAYER_INDEX);
		sp_sharedGame->SetPlayerSnakeColorIndex(static_cast<int>(Enums::Color::Red), SECOND_PLAYER_INDEX);

		mp_playerOneSnakeColorBufferCell->m_character = Consts::EMPTY_SPACE_CHAR;
		mp_playerOneSnakeColorBufferCell->m_colorBFGround = Consts::BACKGROUND_COLORS[sp_sharedGame->GetPlayerSnakeColorIndexRef(FIRST_PLAYER_INDEX)];
		mp_playerOneSnakeColorBufferCell->m_position.m_x = mp_textLines[1]->m_textLineEndColum + 5;	// Arbitrary 5
		mp_playerOneSnakeColorBufferCell->m_position.m_y = mp_textLines[1]->m_position.m_y;			// NOTE: On same row as first snake's color
		m_cells.PushBack(mp_playerOneSnakeColorBufferCell);

		mp_playerTwoSnakeColorBufferCell->m_character = Consts::EMPTY_SPACE_CHAR;
		mp_playerTwoSnakeColorBufferCell->m_colorBFGround = Consts::BACKGROUND_COLORS[sp_sharedGame->GetPlayerSnakeColorIndexRef(SECOND_PLAYER_INDEX)];
		mp_playerTwoSnakeColorBufferCell->m_position.m_x = mp_textLines[2]->m_textLineEndColum + 5;	// Arbitrary 5
		mp_playerTwoSnakeColorBufferCell->m_position.m_y = mp_textLines[2]->m_position.m_y;			// NOTE: On same row as first snake's color
		m_cells.PushBack(mp_playerTwoSnakeColorBufferCell);
	}
	OptionsMenu(const OptionsMenu&) = delete;
	OptionsMenu& operator=(const OptionsMenu&) = delete;

	// Destruction
	~OptionsMenu()
	{
		delete mp_playerOneSnakeColorBufferCell;
		delete mp_playerTwoSnakeColorBufferCell;
	}

protected:
	// Functionality
	virtual int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 3:
		{
			m_currentButtonNumber = Consts::OFF_BY_ONE;
			return Enums::MenuReturn::Return;
		}
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};
	void NextOption() override
	{
		// If not currently in game, allow color to be changed
		if (sp_sharedGame->GetIsInGameSession() == false)
		{
			switch (m_currentButtonNumber)
			{
				// Snake 1 color
			case 1:
				SelectColorWithWrapping(true, true);
				break;
				// Snake 2 color
			case 2:
				SelectColorWithWrapping(true, false);
				break;
			}
		}
	}
	void PreviousOption() override
	{
		// If not currently in game, allow color to be changed
		if (sp_sharedGame->GetIsInGameSession() == false)
		{
			switch (m_currentButtonNumber)
			{
				// Snake 1 color
			case 1:
				SelectColorWithWrapping(false, true);
				break;
				// Snake 2 color
			case 2:
				SelectColorWithWrapping(false, false);
				break;
			}
		}
	}

private:
	// Static Variables
	static constexpr int FIRST_PLAYER_INDEX = Consts::NO_VALUE;
	static constexpr int SECOND_PLAYER_INDEX = Consts::OFF_BY_ONE;

	// Member Variables
	BufferCell* const mp_playerOneSnakeColorBufferCell;
	BufferCell* const mp_playerTwoSnakeColorBufferCell;

	// Functionality
	void SelectColorWithWrapping(bool _nextColor, bool _playerOne)
	{
		if (_playerOne)
		{
			SelectColorWithWrapping(_nextColor, sp_sharedGame->GetPlayerSnakeColorIndexRef(FIRST_PLAYER_INDEX), sp_sharedGame->GetPlayerSnakeColorIndexRef(SECOND_PLAYER_INDEX));
			mp_playerOneSnakeColorBufferCell->m_colorBFGround = Consts::BACKGROUND_COLORS[sp_sharedGame->GetPlayerSnakeColorIndexRef(FIRST_PLAYER_INDEX)];
		}
		else
		{
			SelectColorWithWrapping(_nextColor, sp_sharedGame->GetPlayerSnakeColorIndexRef(SECOND_PLAYER_INDEX), sp_sharedGame->GetPlayerSnakeColorIndexRef(FIRST_PLAYER_INDEX));
			mp_playerTwoSnakeColorBufferCell->m_colorBFGround = Consts::BACKGROUND_COLORS[sp_sharedGame->GetPlayerSnakeColorIndexRef(SECOND_PLAYER_INDEX)];
		}
	}
	void SelectColorWithWrapping(bool _nextColor, int& _changinePlayerSnakeColorIndex, int _otherPlayerSnakeColorIndex)
	{
		if (_nextColor)
		{
			do
			{
				// If out of color range, wrap to the beginning
				if (++_changinePlayerSnakeColorIndex == static_cast<int>(Enums::Color::NumberOfColors))
				{
					_changinePlayerSnakeColorIndex = static_cast<int>(Enums::Color::Black) + Consts::OFF_BY_ONE;
				}

				// While new color index is the same as other player's color index
			} while (_changinePlayerSnakeColorIndex == _otherPlayerSnakeColorIndex);
		}
		else
		{
			do
			{
				// If out of color range, wrap to the end
				if (--_changinePlayerSnakeColorIndex == static_cast<int>(Enums::Color::Black))
				{
					_changinePlayerSnakeColorIndex = static_cast<int>(Enums::Color::NumberOfColors) - Consts::OFF_BY_ONE;
				}

				// While new color index is the same as other player's color index
			} while (_changinePlayerSnakeColorIndex == _otherPlayerSnakeColorIndex);
		}
	}
};

#endif OPTIONS_MENU_H