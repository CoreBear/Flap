#pragma region Includes
#include "Server.h"

#include <thread>
#pragma endregion

#pragma region Initialization
bool Server::Init_Succeeded(bool& _killMe)
{
	// Enable socket options
	char sockOpt = '1';

	// Broadcast
	{
		// Create broadcast sending socket
		{
			m_broadSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (m_broadSocket == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}

			// Set udp socket for broading
			m_winsockResult = setsockopt(m_broadSocket, SOL_SOCKET, SO_BROADCAST, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}
		}

		// Bind broadcast socket to send
		{
			m_broadSockAddrIn.sin_family = AF_INET;
			m_broadSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.mp_myIPAddress);		// Bind to my address
			m_broadSockAddrIn.sin_port = htons(SERVER_BROADCAST_PORT);									// Bind to my port

			m_winsockResult = bind(m_broadSocket, (SOCKADDR*)&m_broadSockAddrIn, sizeof(m_broadSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;

			}

			// HACK: Broadcast for all 0's. Figure out how to get onto other networks
			// Broadcasting out to all on this network and listening on this port
			m_broadSockAddrIn.sin_addr.S_un.S_addr = INADDR_BROADCAST;
		}
	}

	// Communication
	{
		// Create socket
		{
			m_commSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (m_commSocket == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}

			// HACK: Only needed for the same system, which won't happen, unless we're testing
			m_winsockResult = setsockopt(m_commSocket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}
		}

		// Bind socket to receive
		{
			m_commSockAddrIn.sin_family = AF_INET;
			m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.mp_myIPAddress);		// Bind to my address
			m_commSockAddrIn.sin_port = htons(SERVER_COMMUNICATION_PORT);							// Bind to my port

			m_winsockResult = bind(m_commSocket, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}
		}

		// Format broadcast
		{
			const char* walker = mr_sharedNetwork.mp_myIPAddress;
			int stringIndex = 0;

			while (*walker != '\0')
			{
				m_broadcastSendBuffer[stringIndex++] = *walker;

				++walker;
			}

			m_broadcastSendBuffer[stringIndex++] = '|';

			_itoa(SERVER_COMMUNICATION_PORT, &m_broadcastSendBuffer[stringIndex], 10);
		}
	}

	// Start a daemon thread for broadcasting
	std::thread(&Server::SendBroadMessLoop, this).detach();

	return true;
}
#pragma endregion

#pragma region Updates
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

					// If map is not empty, continue to the next client
					else
					{
						continue;
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

}
void Server::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		// NOTE: Container stores information about who it's being sent from
		m_sizeofSockAddr = sizeof(m_commSockAddrIn);
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, 0, (SOCKADDR*)&m_commSockAddrIn, &m_sizeofSockAddr);
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
			throw std::exception("user generated");
		}

		// Client command
		if (m_recvBuffer[0] == '#')
		{
			// If this client has never attempted to communicate with server
			if (m_mapOfClientAddrsConnTypeAndSpecMess.find(m_commSockAddrIn.sin_addr.S_un.S_addr) == m_mapOfClientAddrsConnTypeAndSpecMess.end())
			{
				m_mapOfClientAddrsConnTypeAndSpecMess.emplace(m_commSockAddrIn.sin_addr.S_un.S_addr, MapVal(false));
			}

			// NOTE: All special messages are stored, so client can handle them at the proper time. This will mitigate send/recv confusion
			if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Ping)]) == 0)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_commSockAddrIn.sin_addr.S_un.S_addr].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Ping);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Disconnect)]) == 0)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_commSockAddrIn.sin_addr.S_un.S_addr].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Disconnect);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::GetNumber)]) == 0)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_commSockAddrIn.sin_addr.S_un.S_addr].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::GetNumber);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Join)]) == 0)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_commSockAddrIn.sin_addr.S_un.S_addr].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Join);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Joined)]) == 0)
			{
				m_mapOfClientAddrsConnTypeAndSpecMess[m_commSockAddrIn.sin_addr.S_un.S_addr].m_specialMessageQueue.PushBack(SharedNetwork::SpecialMessage::Joined);
			}
		}

		// Other
		else
		{

		}
	}
}
void Server::Stop()
{
	m_isRunning = false;

	mr_sharedNetwork.m_numOfConnClientsOnServ = '0';

	GenSpecMess(SharedNetwork::SpecialMessage::Disconnect);

	SendCommMessToEveryClient();
}
#pragma endregion

#pragma region Private Functionality
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
				m_mapIterator->second.m_numberOfCyclesSinceLastPing = 0;
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

					mr_sharedNetwork.m_numOfConnClientsOnServ = static_cast<char>(++m_numberOfConnectedClients) + '0';

					GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Joined, m_mapIterator->first);

					GenSpecMess(SharedNetwork::SpecialMessage::SendNumber);

					SendCommMessToEveryClient();
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
		throw std::exception("user generated");
	}
}
void Server::SendBroadMessLoop()
{
	// Daemon thread
	while (true)
	{
		// NOTE: Container stores information about who it's being sent to
		m_winsockResult = sendto(m_broadSocket, m_broadcastSendBuffer, UCHAR_MAX, 0, (SOCKADDR*)&m_broadSockAddrIn, sizeof(m_broadSockAddrIn));
		if (m_winsockResult == SOCKET_ERROR)
		{
			// HACK: Don't need the errno catch right here, just useful for debugging
			switch (m_winsockErrno = WSAGetLastError())
			{
				// Server disconnects
			case WSAEINTR:
			case WSAENOTSOCK:
			case WSANOTINITIALISED:
				return;
				// Right now, just default, but look into all that are required
			default:
				break;
			}

			// Execution shouldn't make it here when everything is working properly
			throw std::exception("user generated");
		}

		// Sleep
		std::this_thread::sleep_for(std::chrono::seconds(m_secondsBetweenBroadcast));
	}
}
void Server::SendCommMessToEveryClient()
{
	for (m_mapSendAllIterator = m_mapOfClientAddrsConnTypeAndSpecMess.begin(); m_mapSendAllIterator != m_mapOfClientAddrsConnTypeAndSpecMess.end(); ++m_mapSendAllIterator)
	{
		m_commSockAddrIn.sin_addr.S_un.S_addr = m_mapSendAllIterator->first;

		SendCommMess();
	}
}
#pragma endregion

#pragma region Destruction
Server::~Server()
{
	// Does nothing, since this id UDP
	shutdown(m_broadSocket, SD_BOTH);

	closesocket(m_broadSocket);
}
#pragma endregion