#pragma region Includes
#include "Server.h"

#include "Consts.h"
#include "SharedGame.h"
#include "Tools.h"
#pragma endregion

#pragma region Loops
void Server::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		// NOTE: Container stores information about who it's being sent from
		m_sizeofSockAddr = sizeof(m_commSockAddrIn);
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, Consts::NO_VALUE, (SOCKADDR*)&m_commSockAddrIn, &m_sizeofSockAddr);
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
		if (m_recvBuffer[0] == '#')
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
			else if (CheckForJoin())
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

		// So the server doesn't do this all too rapidly
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
#pragma endregion

#pragma region Public Functionality
void Server::Join()
{
	mp_sharedGame->ResetLargestClientOffsets();

	// Get the largest game area possible, that's common for all players
	for (m_mapJoinIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapJoinIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapJoinIterator)
	{
		mp_sharedGame->UpdateClientSharedGameAreaOffsets(m_mapIterator->second.m_maxFrameBufferWidthHeight);
	}
}
void Server::Stop()
{
	m_isRunning = false;

	mr_sharedNetwork.m_numOfConnClientsOnServ = '0';

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
bool Server::CheckForJoin()
{
	mp_recvBuffWalker = m_recvBuffer;
	mp_specMessWalker = mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Join)];

	while (*mp_recvBuffWalker != '|')
	{
		// If any character is off, this is not the SendNumber response
		if (*mp_recvBuffWalker != *mp_specMessWalker)
		{
			return false;
		}

		++mp_recvBuffWalker;
		++mp_specMessWalker;
	}

	// To get beyond the pipe character
	++mp_recvBuffWalker;

	mp_joinFrameBufferDimensionsPipeNuller = mp_recvBuffWalker;

	// Move deleter down to pipe
	while (*mp_joinFrameBufferDimensionsPipeNuller != '|')
	{
		++mp_joinFrameBufferDimensionsPipeNuller;
	}

	// Null pipe
	*mp_joinFrameBufferDimensionsPipeNuller = '\0';

	return true;
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
				if (m_numberOfConnectedClients < MAX_NUMBER_OF_CLIENTS)
				{
					m_mapIterator->second.m_joined = true;

					m_mapIterator->second.m_maxFrameBufferWidthHeight.m_x = static_cast<short>(Tools::StringToInt(mp_recvBuffWalker));

					mp_recvBuffWalker = mp_joinFrameBufferDimensionsPipeNuller;
					++mp_recvBuffWalker;

					m_mapIterator->second.m_maxFrameBufferWidthHeight.m_y = static_cast<short>(Tools::StringToInt(mp_recvBuffWalker));

					mr_sharedNetwork.m_numOfConnClientsOnServ = static_cast<char>(++m_numberOfConnectedClients) + '0';

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
		mr_sharedNetwork.m_numOfConnClientsOnServ = static_cast<char>(--m_numberOfConnectedClients) + '0';
	}

	_iterator = m_mapOfClientAddrsConnTypeAndSpecMess.erase(_iterator);

	GenSpecMess(SharedNetwork::SpecialMessage::SendNumber);

	SendCommMessToEveryClient_Except();

	return _iterator == m_mapOfClientAddrsConnTypeAndSpecMess.end();
}
void Server::SendCommMess()
{
	// NOTE: Container stores information about who it's being sent to
	m_winsockResult = sendto(m_commSocket, m_sendBuffer, UCHAR_MAX, 0, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
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
#pragma endregion