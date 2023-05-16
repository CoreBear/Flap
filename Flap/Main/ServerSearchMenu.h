#ifndef SERVER_SEARCH_MENU_H
#define SERVER_SEARCH_MENU_H

#include "BufferCell.h"
#include "Consts.h"
#include "Defines.h"
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
		// Initialize the dots
		NetworkSearchMenu::Initialize();

		mp_newString = new char[SharedGame::MAX_HS_STRING_LENGTH];
		strcpy(mp_newString, "Server IP: ");

#ifdef TEST_ON_LOOP_BACK
		strcat(mp_newString, mr_sharedNetwork.LOOP_BACK_ADDR);

		// If IP has not already been entered
		mr_sharedNetwork.m_serverIPAddressMutex.lock();
		if (mr_sharedNetwork.m_serverIPIsEmpty)
		{
			mr_sharedNetwork.m_serverIPIsEmpty = false;

			mp_walker = mr_sharedNetwork.LOOP_BACK_ADDR;
			m_reusableIterator = Consts::NO_VALUE;

			while (*mp_walker != '\0')
			{
				mr_sharedNetwork.m_serverIPAddress[m_reusableIterator++] = *mp_walker;

				++mp_walker;
			}
		}
		mr_sharedNetwork.m_serverIPAddressMutex.unlock();
#else !TEST_ON_LOOP_BACK
		// If user hasn't tried entering an IP
		mr_sharedNetwork.m_serverIPAddressMutex.lock();
		if (mr_sharedNetwork.m_serverIPIsEmpty)
		{
			mr_sharedNetwork.m_serverIPAddressMutex.unlock();

			strcat(mp_newString, "___.___.___.___");
		}

		// If user has alread tried entering an IP
		else
		{

			strcat(mp_newString, mr_sharedNetwork.m_serverIPAddress);

			mr_sharedNetwork.m_serverIPAddressMutex.unlock();
		}
#endif TEST_ON_LOOP_BACK

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
		{
			for (m_ipCellIterator = m_cells.Begin(); m_ipCellIterator != m_cells.End(); ++m_ipCellIterator)
			{
				if ((*m_ipCellIterator)->m_character == ':')
				{
					break;
				}
			}

			m_ipCellIterator += 2;
		}

		(*m_ipCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];

		// Point at the current last node, so this iterator can move to the next node when it's created
		m_messageBufferStartIterator = m_cells.GetTail();

		// We do not care about the x-position, because it will change, based on the message
		m_columnPositionOffset = Consts::NO_VALUE;

		// HACK: Hardcoded. 
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < 100; m_reusableIterator++)
		{
			constexpr int ROW = 20;

			mp_newBufferCell = new BufferCell;
			mp_newBufferCell->m_character = Consts::EMPTY_SPACE_CHAR;
			mp_newBufferCell->m_colorBFGround = NULL;
			mp_newBufferCell->m_position.m_x = m_columnPositionOffset++;
			mp_newBufferCell->m_position.m_y = ROW;
			m_cells.PushBack(mp_newBufferCell);

			// HACK: Hardcoded
			// Store the cell that will be updated when a user conncets
			if (m_reusableIterator == 42)
			{
				mp_connectedUsersBufferCell = mp_newBufferCell;
			}
		}
		
		// Move iterator to the start of the message buffer
		++m_messageBufferStartIterator;

		m_ipIndex = 0;

		delete[] mp_newString;
		mp_newString = nullptr;
	}
	ServerSearchMenu(SharedNetwork& _sharedNetwork) :
		NetworkSearchMenu(3, _sharedNetwork),			// This value must match the number of text lines below
		mp_messageBuffer(new char[100]),				// HACK: Hardcoded
		mpp_cannedMessages(new const char*[static_cast<int>(SharedNetwork::ClientState::NumberOfTotalStates)]),
		mp_cannedMessageStartIndices(new int[static_cast<int>(SharedNetwork::ClientState::NumberOfTotalStates)])
	{
		mp_textLines = new TextLine * [m_totNumOfTextLines]
		{
			new TextLine("Searching For Server", Consts::OFF_BY_ONE),	// Menu Title
			//new TextLine("...", 2),
			//new TextLine("Server IP: ", 5),
			new TextLine("Return to network menu", 10),
			new TextLine("Join server", 15)
			//new TextLine("A message to the user about what's occurring (e.g. Joined server...Number Of Connected Users: 0/4)", 20)
		};

		// Clear buffer
		memset(mp_messageBuffer, Consts::EMPTY_SPACE_CHAR, 100);

		// Generate messages
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::AttemptToJoinServ)] = "Attempting to join server...";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::JoinedServ_InGame)] = nullptr;
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::JoinedServ_InLobby)] = "Joined server. Number of connected users: 0/4";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::JoinedServ_PreGame)] = nullptr;
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::NotJoined)] = "Please enter an IP and join server.";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::CouldNotConnect)] = "Could not connect to this server. Please try a different IP.";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::FullServ)] = "This server is full. Please try a different IP.";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::InvalidAddr)] = "Invalid IP address supplied. Please try a different IP.";
		mpp_cannedMessages[static_cast<int>(SharedNetwork::ClientState::ServDisc)] = "Server disconnected. Please try a new IP.";

		// Generate start indices
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < static_cast<int>(SharedNetwork::ClientState::NumberOfTotalStates); m_reusableIterator++)
		{
			// If there's a message to offset
			if (mpp_cannedMessages[m_reusableIterator] != nullptr)
			{
				mp_cannedMessageStartIndices[m_reusableIterator] = Tools::CenterText_ReturnStartColumn(mpp_cannedMessages[m_reusableIterator]);
			}
		}
	}
	ServerSearchMenu(const ServerSearchMenu&) = delete;
	ServerSearchMenu& operator=(const ServerSearchMenu&) = delete;

	//Updates
	void FixedUpdate() override
	{
		if (NetworkSearchMenu::UpdateDots())
		{
			// If menu should be updating
			mr_sharedNetwork.m_nextClientStateMutex.lock();
			if (mr_sharedNetwork.m_waitForMenuUpdate)
			{
				mr_sharedNetwork.m_waitForMenuUpdate = false;

				m_clientStateIndex = static_cast<int>(mr_sharedNetwork.m_nextClientState);

				mr_sharedNetwork.m_nextClientStateMutex.unlock();
				
					// Generate message
				strcpy(mp_messageBuffer, mpp_cannedMessages[m_clientStateIndex]);

				m_columnPositionOffset = mp_cannedMessageStartIndices[m_clientStateIndex];

				// Start at the beginning (of the message and message buffer cells)
				mp_walker = mp_messageBuffer;
				m_messageBufferCellIterator = m_messageBufferStartIterator;

				// For each character in the message, write it out
				while (*mp_walker != '\0')
				{
					(*m_messageBufferCellIterator)->m_character = *mp_walker;
					(*m_messageBufferCellIterator)->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];
					(*m_messageBufferCellIterator)->m_position.m_x = m_columnPositionOffset++;

					++mp_walker;
					++m_messageBufferCellIterator;
				}

				// For each non-character in the message, make blank
				for (/*Initialization occurs above*/; m_messageBufferCellIterator != m_cells.End(); ++m_messageBufferCellIterator)
				{
					(*m_messageBufferCellIterator)->m_character = Consts::EMPTY_SPACE_CHAR;
					(*m_messageBufferCellIterator)->m_colorBFGround = NULL;
					(*m_messageBufferCellIterator)->m_position.m_x = m_columnPositionOffset++;
				}
			}
			else
			{
				mr_sharedNetwork.m_nextClientStateMutex.unlock();
			}

			constexpr int MENU_THAT_DISPLAYS_CONN_CLIENTS = static_cast<int>(SharedNetwork::ClientState::JoinedServ_InLobby);
			if (m_clientStateIndex == MENU_THAT_DISPLAYS_CONN_CLIENTS)
			{
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.lock();
				mp_connectedUsersBufferCell->m_character = mr_sharedNetwork.m_numOfConnClientsOnServChar;
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();
			}
		}
	}

	// Destruction
	virtual ~ServerSearchMenu() override 
	{
		delete[] mp_messageBuffer;
		delete[] mpp_cannedMessages;
		delete[] mp_cannedMessageStartIndices;
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
	void InputCharacter(int _inputIndexOrKeyCode) override
	{
#ifndef TEST_ON_LOOP_BACK
		(*m_ipCellIterator)->m_character = static_cast<char>(_inputIndexOrKeyCode);
		mr_sharedNetwork.m_serverIPAddressMutex.lock();
		mr_sharedNetwork.m_serverIPIsEmpty = false;
		mr_sharedNetwork.m_serverIPAddress[m_ipIndex] = static_cast<char>(_inputIndexOrKeyCode);
		mr_sharedNetwork.m_serverIPAddressMutex.unlock();
#endif TEST_ON_LOOP_BACK

		NextOption();
	}
	void NextOption() override
	{
		// If index can move right, move it right
		if (m_ipIndex < SharedNetwork::IP_ADDR_CHAR_LENGTH - Consts::OFF_BY_ONE)
		{
			// Unhighlight the previous option 
			(*m_ipCellIterator)->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];

			// Move once
			++m_ipIndex;
			++m_ipCellIterator;

			// If a dot, move again
			if ((*m_ipCellIterator)->m_character == DOT_CHAR)
			{
				++m_ipIndex;
				++m_ipCellIterator;
			}

			// Highlight the current option
			(*m_ipCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
		}
	}
	void PreviousOption() override
	{
		// If index can move left, move it left
		if (m_ipIndex > 0)
		{
			// Unhighlight the previous option 
			(*m_ipCellIterator)->m_colorBFGround = Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::White)];

			// Move once
			--m_ipIndex;
			--m_ipCellIterator;

			// If a dot, move again
			if ((*m_ipCellIterator)->m_character == DOT_CHAR)
			{
				--m_ipIndex;
				--m_ipCellIterator;
			}

			// Highlight the current option
			(*m_ipCellIterator)->m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::White)];
		}
	}

private:
	// Static Variables
	static constexpr char DOT_CHAR = '.';

	// Member Variables
	char* mp_messageBuffer;
	const char** mpp_cannedMessages;
	DList<BufferCell*>::Const_Iterator m_messageBufferStartIterator;
	DList<BufferCell*>::Iterator m_messageBufferCellIterator;
	DList<BufferCell*>::Iterator m_ipCellIterator;
	int m_clientStateIndex;
	int m_ipIndex;
	int m_reusableIterator;
	int* mp_cannedMessageStartIndices;
};

#endif SERVER_SEARCH_MENU_H