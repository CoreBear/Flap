#pragma region Includes
#include "Server.h"

#include "BufferCell.h"
#include "Consts.h"
#include "Enums.h"
#include "SharedGame.h"
#include "Tools.h"
#pragma endregion

#pragma region Initialization
Server::Server(SharedGame& _sharedGame, SharedNetwork& _sharedNetwork) :
	Host(1, _sharedGame, _sharedNetwork),							// HACK: Hardcoding
	m_isInGame(false),
	m_frameBufferUniqueLock(_sharedGame.m_frameBufferMutex)
{
	m_frameBufferUniqueLock.unlock();

	mr_sharedNetwork.m_numOfConnClientsOnServChar = static_cast<char>(mr_sharedNetwork.m_numOfConnClientsOnServInt = 0) + '0';
}
#pragma endregion

#pragma region Loops
void Server::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		// NOTE: Container stores information about who it's being sent from
		m_sizeofSockAddr = sizeof(m_commSockAddrIn);
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, MAX_BUFF_SIZE, Consts::NO_VALUE, (SOCKADDR*)&m_commSockAddrIn, &m_sizeofSockAddr);
		if (m_winsockResult == SOCKET_ERROR)
		{
			// HACK: Don't need the errno catch right here, just useful for debugging
			switch (m_winsockErrno = WSAGetLastError())
			{
				// If client disconnected, remove them and continue on without handling their message
			case WSAECONNRESET:
				continue;
				// Server disconnects
			case WSAEINTR:
				return;
				// Right now, just default, but look into all that are required
			default:
				break;
			}

			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
		}

		// Client command
		constexpr char COMMAND_CHAR = '#';
		if (m_recvBuffer[0] == COMMAND_CHAR)
		{
#ifdef SAME_SYSTEM_NETWORK
			m_sendingClientsAddrPort = static_cast<unsigned long>(m_commSockAddrIn.sin_port);
#else !SAME_SYSTEM_NETWORK
			m_sendingClientsAddrPort = m_commSockAddrIn.sin_addr.S_un.S_addr;
#endif SAME_SYSTEM_NETWORK

			// If this client's IP address is not found, the client has never attempted to communicate with server and a new pair should be created and added
			if (m_mapOfClientAddrsConnTypeAndSpecMess.find(m_sendingClientsAddrPort) == m_mapOfClientAddrsConnTypeAndSpecMess.end())
			{
				m_mapOfClientAddrsConnTypeAndSpecMess.emplace(m_sendingClientsAddrPort, MapVal(false));
			}

			// NOTE: All special messages are stored, so client can handle them at the proper time. This will mitigate send/recv confusion
			if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Ping)]) == Consts::NO_VALUE)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_sendingClientsAddrPort].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Ping);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Disconnect)]) == Consts::NO_VALUE)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_sendingClientsAddrPort].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Disconnect);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::GetNumber)]) == Consts::NO_VALUE)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_sendingClientsAddrPort].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::GetNumber);
			}
			else if (CheckForSpecMess(SharedNetwork::SpecialMessage::Join))
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_sendingClientsAddrPort].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Join);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Joined)]) == Consts::NO_VALUE)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_sendingClientsAddrPort].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Joined);
			}
		}

		// Other
		else
		{

		}
	}
}
void Server::UpdateLoop()
{
	while (m_isRunning)
	{
		// For each joined client
		for (m_mapIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapIterator)
		{
			if (m_mapIterator->second.m_joined)
			{
				// If client hasn't ping'ed recently enough, remove it
				constexpr int MAX_NUM_CYC_SINCE_LAST_PING = 3;
				if (++m_mapIterator->second.m_numberOfCyclesSinceLastPing > MAX_NUM_CYC_SINCE_LAST_PING)
				{
					// If map is empty, stop iterating
					if (RemoveClient_EmptyMap(m_mapIterator))
					{
						break;
					}
				}
			}
		}

		// Handle the special messages of all clients
		HandleSpecMess();

		if (m_isInGame)
		{
			StoreGameBoard();

			SendCommMessToEveryClient_Except();

			// The server 
			//constexpr long long HALF_FRAME_DELTA_TIME = static_cast<long long>(Consts::FIXED_DELTA_TIME_LL * Consts::MULTIPLICATIVE_HALF_F);
			//std::this_thread::sleep_for(std::chrono::microseconds(Consts::FIXED_DELTA_TIME_LL));
			//std::this_thread::sleep_for(std::chrono::microseconds(HALF_FRAME_DELTA_TIME)); Consts::FIXED_DELTA_TIME_LL
		}
		else
		{
			// So the server doesn't do this all too rapidly
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}
#pragma endregion

#pragma region Public Functionality
void Server::Join()
{
	mp_sharedGame->m_clientSharedGameAreaOffsets.m_x = SHRT_MAX;
	mp_sharedGame->m_clientSharedGameAreaOffsets.m_y = SHRT_MAX;
	
	// Get the largest game area possible, that's common for all players
	for (m_mapJoinIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapJoinIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapJoinIterator)
	{
		mp_sharedGame->UpdateClientSharedGameAreaOffsets(m_mapJoinIterator->second.m_maxFrameBufferWidthHeight);
	}

	mr_sharedNetwork.m_startNetworkedGameMutex.lock();
	mr_sharedNetwork.m_startNetworkedGame = true;
	mr_sharedNetwork.m_startNetworkedGameMutex.unlock();

	m_isInGame = true;

	GenSpecMess(SharedNetwork::SpecialMessage::StartGame);

	SendCommMessToEveryClient_Except();
}
void Server::Stop()
{
	m_isRunning = false;

	mr_sharedNetwork.m_numOfConnClientsOnServChar = '0';

	GenSpecMess(SharedNetwork::SpecialMessage::Disconnect);

	SendCommMessToEveryClient_Except();
}
#pragma endregion

#ifdef SAME_SYSTEM_NETWORK
#pragma region Protected Functionality
void Server::AssignPort()
{
	m_commSockAddrIn.sin_port = htons(SERVER_COMMUNICATION_PORT); // Bind to my port
}
#pragma endregion
#endif SAME_SYSTEM_NETWORK

#pragma region Private Functionality
void Server::AddrAndSendCommMess(unsigned long _addressOrPort)
{
#ifdef SAME_SYSTEM_NETWORK
	m_commSockAddrIn.sin_port = static_cast<unsigned short>(_addressOrPort);
#else !SAME_SYSTEM_NETWORK
	m_commSockAddrIn.sin_addr.S_un.S_addr = _addressOrPort;
#endif SAME_SYSTEM_NETWORK

	SendCommMess();
}
void Server::HandleSpecMess()
{
	// For each client
	for (m_mapIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapIterator)
	{
		// While client has commands
		while (m_mapIterator->second.m_specialMessageQueue.IsEmpty() == false)
		{
			switch (m_mapIterator->second.m_specialMessageQueue.Peek())
			{
			case SharedNetwork::SpecialMessage::Ping:
			{
				m_mapIterator->second.m_numberOfCyclesSinceLastPing = Consts::NO_VALUE;
			}
			break;
			case SharedNetwork::SpecialMessage::Disconnect:
			{
				// If map is empty, stop iterating
				if (RemoveClient_EmptyMap(m_mapIterator))
				{
					return;
				}

				// If map is not empty, continue to the next client
				else
				{
					continue;
				}
			}
			case SharedNetwork::SpecialMessage::Join:
			{
				// HACK: Put this const somewhere else
				constexpr int MAX_NUMBER_OF_CLIENTS = 4;
				if (mr_sharedNetwork.m_numOfConnClientsOnServInt < MAX_NUMBER_OF_CLIENTS)
				{
					m_mapIterator->second.m_joined = true;

					NextBufferString(false);
					m_mapIterator->second.m_maxFrameBufferWidthHeight.m_x = static_cast<short>(Tools::StringToInt(mp_recvBuffWalkerTrailing));

					NextBufferString(false);
					m_mapIterator->second.m_maxFrameBufferWidthHeight.m_y = static_cast<short>(Tools::StringToInt(mp_recvBuffWalkerTrailing));

					mr_sharedNetwork.m_numOfConnClientsOnServChar = static_cast<char>(++mr_sharedNetwork.m_numOfConnClientsOnServInt) + '0';

					GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Joined, m_mapIterator->first);

					GenSpecMess(SharedNetwork::SpecialMessage::SendNumber);

					SendCommMessToEveryClient_Except();
				}
				else
				{
					GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Full, m_mapIterator->first);
				}
			}
			break;
			case SharedNetwork::SpecialMessage::GetNumber:
				GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::SendNumber, m_mapIterator->first);
				break;
			}

			m_mapIterator->second.m_specialMessageQueue.PopFront();
		}
	}
}
bool Server::RemoveClient_EmptyMap(std::unordered_map<unsigned long, Server::MapVal>::iterator& _iterator)
{
	if (_iterator->second.m_joined)
	{
		mr_sharedNetwork.m_numOfConnClientsOnServChar = static_cast<char>(--mr_sharedNetwork.m_numOfConnClientsOnServInt) + '0';
	}

	_iterator = m_mapOfClientAddrsConnTypeAndSpecMess.erase(_iterator);

	GenSpecMess(SharedNetwork::SpecialMessage::SendNumber);

	SendCommMessToEveryClient_Except();

	return _iterator == m_mapOfClientAddrsConnTypeAndSpecMess.end();
}
void Server::SendCommMess()
{
	// NOTE: Container stores information about who it's being sent to
	m_winsockResult = sendto(m_commSocket, m_sendBuffer, MAX_BUFF_SIZE, 0, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
	if (m_winsockResult == SOCKET_ERROR)
	{
		// HACK: Don't need the errno catch right here, just useful for debugging
		switch (m_winsockErrno = WSAGetLastError())
		{
			// Right now, just default, but look into all that are required
		default: 
			break;
		}

		// Execution shouldn't make it here when everything is working properly
		throw std::exception();
	}
}
void Server::SendCommMessToEveryClient_Except(unsigned long _address)
{
	// If an address comes in
	if (_address != ULONG_MAX)
	{
		// For each connected client
		for (m_mapSendAllIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapSendAllIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapSendAllIterator)
		{
			// If this client's address is not the one to be excluded, assign address and send message
			if (m_mapSendAllIterator->first != _address)
			{
				AddrAndSendCommMess(m_mapSendAllIterator->first);
			}
		}
	}

	// If no address comes int
	else
	{
		// For each connected client, assign address and send message
		for (m_mapSendAllIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapSendAllIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapSendAllIterator)
		{
			AddrAndSendCommMess(m_mapSendAllIterator->first);
		}
	}
}
void Server::StoreGameBoard()
{
	m_frameBufferUniqueLock.lock();
	mp_sharedGame->m_serverConVar.wait(m_frameBufferUniqueLock);

	m_cellIndex = Consts::NO_VALUE;

	for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
	{
		for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
		{
			// First 4 bits (lower) are char (number in cell)
			// Store low bits
			m_sendBuffer[m_cellIndex] = mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_character;

			// If empty space, make it null (will be translated on the other side)
			if (m_sendBuffer[m_cellIndex] == Consts::EMPTY_SPACE_CHAR)
			{
				m_sendBuffer[m_cellIndex] = NULL;
			}

			// If NoTouchy
			else if (m_sendBuffer[m_cellIndex] == NO_TOUCHY_CHAR)
			{
				m_sendBuffer[m_cellIndex++] = CHAR_MAX;
				continue;
			}

			// If food (has a number)
			else
			{
				m_sendBuffer[m_cellIndex] -= ASCII_CHAR_OFFSETTER;
			}

			// Second 4 bits (higher) are color (background, foreground will be black)
			// Store high bits in a temp variable
			m_colorBits = mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_colorIndex;

			// Shift low bits to high bits
			m_colorBits <<= LOW_HIGH_BIT_SHIFT;

			// Combine number (low) and color (high) bits
			m_sendBuffer[m_cellIndex++] |= m_colorBits;
		}
	}

	m_frameBufferUniqueLock.unlock();

	// Release render thread
	mp_sharedGame->m_rendererConVar.notify_one();
}
#pragma endregion