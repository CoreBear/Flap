#ifndef CLIENT_WAIT_MENU_H
#define CLIENT_WAIT_MENU_H

#include "BufferCell.h"
#include "NetworkSearchMenu.h"
#include "SharedNetwork.h"
#include "TextLine.h"

class SharedNetwork;

class ClientWaitMenu final : public NetworkSearchMenu
{
public:
	// Initialization
	ClientWaitMenu(SharedNetwork& _sharedNetwork) :
		NetworkSearchMenu(3, _sharedNetwork),			// This value must match the number of text lines below
		m_lobbyWasEmptyLastFrame(true)
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Waiting For Players", Consts::OFF_BY_ONE),	// Menu Title
			//new TextLine("...", 2),
			//new TextLine("Number Of Connected Users: 0/4", 10),
			new TextLine("Return to network menu", 15),
			new TextLine("Start Game", 20)
		};

		// HACK: Do this dynamically. This is so the system only displays the first 2 options at first
		m_currNumOfTextLines = 2;
	}
	ClientWaitMenu(const ClientWaitMenu&) = delete;
	ClientWaitMenu& operator=(const ClientWaitMenu&) = delete;
	void Initialize() override
	{
		// Initialize the dots
		NetworkSearchMenu::Initialize();

		mp_newString = new char[SharedGame::MAX_HS_STRING_LENGTH];
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
		mp_newString = nullptr;
	}

	//Updates
	void FixedUpdate() override
	{
		if (NetworkSearchMenu::UpdateDots())
		{
			// Number Of Connected Users
			{
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.lock();
				mp_connectedUsersBufferCell->m_character = mr_sharedNetwork.m_numOfConnClientsOnServChar;
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();
			}

			// If lobby is empty
			mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.lock();
			if (mr_sharedNetwork.m_numOfConnClientsOnServChar == '0')
			{
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();

				if (m_lobbyWasEmptyLastFrame == false)
				{
					m_lobbyWasEmptyLastFrame = true;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(2);
				}
			}

			// If lobby is not empty
			else
			{
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();

				if (m_lobbyWasEmptyLastFrame)
				{
					m_lobbyWasEmptyLastFrame = false;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(3);
				}
			}
		}
	}

protected:
	// Functionality
	int InputAcceptHandling() override
	{
		switch (m_currentButtonNumber)
		{
		case 1:
			return Enums::MenuReturn::StopHost;
		case 2:
			return Enums::MenuReturn::Join;
		}

		// NOTE: If player clicks accept on a non-button
		return Enums::MenuReturn::NA;
	};

private:
	// Member Variables
	bool m_lobbyWasEmptyLastFrame;
};

#endif CLIENT_WAIT_MENU_H