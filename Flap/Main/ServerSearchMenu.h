#ifndef SERVER_SEARCH_MENU_H
#define SERVER_SEARCH_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "DList.h"
#include "NetworkSearchMenu.h"
#include "SharedNetwork.h"
#include "TextLine.h"

class ServerSearchMenu final : public NetworkSearchMenu
{
public:
	// Initialization
	void Initialize() override
	{
		// Dots and Number of Connected Users
		NetworkSearchMenu::Initialize();

		mp_newString = new char[sp_sharedGame->MAX_HS_STRING_LENGTH];
		strcpy(mp_newString, "Server IP: ???.???.???.???");

		mp_walker = mp_newString;

		m_columnPositionOffset = Tools::CenterText_ReturnStartColumn(mp_newString);

		while (*mp_walker != '\0')
		{
			constexpr int ROW = 5;

			mp_newBufferCell = new BufferCell;
			mp_newBufferCell->m_character = *mp_walker;
			mp_newBufferCell->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
			mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
			mp_newBufferCell->m_position.m_y = ROW;
			m_cells.PushBack(mp_newBufferCell);

			++mp_walker;
		}

		// Position iterator at the beginning of the IP address
		for (m_ipStartIter = m_cells.Begin(); m_ipStartIter != m_cells.End(); ++m_ipStartIter)
		{
			if ((*m_ipStartIter)->m_character == '?')
			{
				break;
			}
		}

		// HACK: Hardcoded
		UpdateNumberOfTextLines(2);
		m_clientConnectedToServer = false;
		m_couldSeeServerLastUpdate = false;

		delete[] mp_newString;
	}
	ServerSearchMenu(SharedNetwork& _sharedNetwork) :
		NetworkSearchMenu(3, _sharedNetwork),			// This value must match the number of text lines below
		m_clientConnectedToServer(false),
		m_couldSeeServerLastUpdate(false)
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Searching For Server", Consts::OFF_BY_ONE),	// Menu Title
				//new TextLine("...", 2),
				//new TextLine("Server IP: ", 5),
				//new TextLine("Number Of Connected Users: 0/4", 10),
				new TextLine("Return to network menu", 15),
				new TextLine("Connect to server", 20)
		};

		// HACK: Do this dynamically. This is so the system only displays the first 2 options at first
		m_currNumOfTextLines = 2;
	}
	ServerSearchMenu(const ServerSearchMenu&) = delete;
	ServerSearchMenu& operator=(const ServerSearchMenu&) = delete;

	//Updates
	void FixedUpdate() override
	{
		if (NetworkSearchMenu::UpdateDotsAndConnectedUsers())
		{
			m_ipUpdateIter = m_ipStartIter;

			constexpr int NUMBER_OF_IP_CHARACTERS = 15;

			// If client sees server
			mr_sharedNetwork.m_serverIPAddressMutex.lock();
			if (mr_sharedNetwork.mp_serverIPAddress != nullptr)
			{
				mr_sharedNetwork.m_serverIPAddressMutex.unlock();

				if (m_couldSeeServerLastUpdate == false)
				{
					m_couldSeeServerLastUpdate = true;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(3);
				}

				// If client could see server last frame
				else
				{
					// If this menu doesn't know the client has connected
					if (m_clientConnectedToServer == false)
					{
						// If client connected to server
						mr_sharedNetwork.m_joinedServerMutex.lock();
						if (mr_sharedNetwork.m_joinedServer == true)
						{
							m_clientConnectedToServer = true;

							UpdateNumberOfTextLines(2);

							ResetButtonNumber();
						}
						mr_sharedNetwork.m_joinedServerMutex.unlock();
					}
				}

				m_ipWalker = mr_sharedNetwork.mp_serverIPAddress;

				m_reusableIterator = Consts::NO_VALUE;

				while (*m_ipWalker != '\0')
				{
					(*m_ipUpdateIter)->m_character = *m_ipWalker;

					++m_ipWalker;
					++m_ipUpdateIter;
					++m_reusableIterator;
				}

				for (; m_reusableIterator < NUMBER_OF_IP_CHARACTERS; m_reusableIterator++)
				{
					(*m_ipUpdateIter)->m_character = Consts::EMPTY_SPACE_CHAR;
					++m_ipUpdateIter;
				}
			}

			// If client does not see server
			else
			{
				mr_sharedNetwork.m_serverIPAddressMutex.unlock();

				if (m_couldSeeServerLastUpdate)
				{
					m_couldSeeServerLastUpdate = false;

					// HACK: Hardcoded
					UpdateNumberOfTextLines(2);
				}

				for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < NUMBER_OF_IP_CHARACTERS; m_reusableIterator++)
				{
					(*m_ipUpdateIter)->m_character = (m_reusableIterator % 4 != 3) ? '?' : '.';
					++m_ipUpdateIter;
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
	bool m_clientConnectedToServer;
	bool m_couldSeeServerLastUpdate;
	const char* m_ipWalker;
	DList<BufferCell*>::Const_Iterator m_ipStartIter;
	DList<BufferCell*>::Iterator m_ipUpdateIter;
	int m_reusableIterator;
};

#endif SERVER_SEARCH_MENU_H