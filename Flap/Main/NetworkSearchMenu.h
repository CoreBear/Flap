#ifndef NETWORK_SEARCH_MENU_H
#define NETWORK_SEARCH_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DynamicTextMenu.h"
#include "GameManager.h"
#include "SharedGame.h"
#include "SharedNetwork.h"

class NetworkSearchMenu : public DynamicTextMenu
{
public:
	// Initialization
	void Initialize() override
	{
		ClearCells();

		// Dots
		{
			mp_newString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
			strcpy(mp_newString, "...");

			mp_walker = mp_newString;

			m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

			while (*mp_walker != '\0')
			{
				constexpr int ROW = 2;

				mp_newBufferCell = new BufferCell;
				mp_newBufferCell->m_character = *mp_walker;
				mp_newBufferCell->m_colorBFGround = (mp_walker == mp_newString) ? Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)] : NULL;
				mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
				mp_newBufferCell->m_position.m_y = ROW;
				m_cells.PushBack(mp_newBufferCell);

				++mp_walker;
			}

			m_dotUpdateTargetFrame = GameManager::s_masterFixedFrameCount + m_numberOfFramesBetweenDotUpdate;
			m_numberOfDotsOn = Consts::OFF_BY_ONE;

			delete[] mp_newString;
		}

		// Number Of Connected Users
		{

			mp_newString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
			strcpy(mp_newString, "Number Of Connected Users: 0/4");

			mp_walker = mp_newString;

			m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

			while (*mp_walker != '\0')
			{
				constexpr int ROW = 10;

				mp_newBufferCell = new BufferCell;
				mp_newBufferCell->m_character = *mp_walker;
				mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
				mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
				mp_newBufferCell->m_position.m_y = ROW;
				m_cells.PushBack(mp_newBufferCell);

				// Point at the cell that will be updated
				if (*mp_walker == '0')
				{
					mp_connectedUsersBufferCell = mp_newBufferCell;
				}

				++mp_walker;
			}

			delete[] mp_newString;
		}
	}

protected:
	// Member Variables
	SharedNetwork& mr_sharedNetwork;
	
	// Initialization
	inline NetworkSearchMenu(int _numberOfTextLines, SharedNetwork& _sharedNetwork) :
		DynamicTextMenu(_numberOfTextLines),
		m_numberOfFramesBetweenDotUpdate(60),	// Arbirtary 60
		mr_sharedNetwork(_sharedNetwork)
	{ 
		return;
	}
	NetworkSearchMenu(const NetworkSearchMenu&) = delete;
	NetworkSearchMenu& operator=(const NetworkSearchMenu&) = delete;
	
	// Functionality
	bool UpdateDotsAndConnectedUsers()
	{
		if (GameManager::s_masterFixedFrameCount == m_dotUpdateTargetFrame)
		{
			// Dots
			{
				m_dotUpdateTargetFrame = GameManager::s_masterFixedFrameCount + m_numberOfFramesBetweenDotUpdate;

				// Figure out the number of dots that should be on
				constexpr int NUMBER_OF_DOTS = 3;
				if (++m_numberOfDotsOn > NUMBER_OF_DOTS)
				{
					m_numberOfDotsOn = Consts::OFF_BY_ONE;
				}

				m_dotIndex = Consts::NO_VALUE;

				for (m_updateCellIterator = m_cells.Begin(); m_updateCellIterator != m_cells.End(); ++m_updateCellIterator)
				{
					// Color dot indices appropriately
					if (m_dotIndex < NUMBER_OF_DOTS)
					{
						(*m_updateCellIterator)->m_colorBFGround = (m_dotIndex++ < m_numberOfDotsOn) ? Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)] : NULL;
					}

					// If out of the dot index, stop altering things
					else
					{
						break;
					}
				}
			}

			// Number Of Connected Users
			{
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.lock();
				mp_connectedUsersBufferCell->m_character = mr_sharedNetwork.m_numOfConnClientsOnServ;
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.unlock();
			}

			// Let system know dots were updated
			return true;
		}

		// Let system know dots were not updated
		return false;
	}

private:
	// Member Variables
	BufferCell* mp_connectedUsersBufferCell;
	DList<BufferCell*>::Iterator m_updateCellIterator;
	int m_dotIndex;
	int m_numberOfDotsOn;
	unsigned int m_dotUpdateTargetFrame;
	unsigned int m_numberOfFramesBetweenDotUpdate;
};

#endif NETWORK_SEARCH_MENU_H