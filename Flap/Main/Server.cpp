#pragma region Includes
#include "Server.h"

#include <thread>
#pragma endregion

#pragma region Initialization
Server::Server(SharedNetwork& _sharedNetwork) : 
	Host(_sharedNetwork),
	m_secondsBetweenBroadcast(NORMAL_BROADCAST_WAIT)
{
	char sockOpt = '1';

	// Broadcast
	{
		// Create broadcast sending socket
		{
			m_broadSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (m_broadSocket == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();
				return;
			}

			// Set udp socket for broading
			m_winsockResult = setsockopt(m_broadSocket, SOL_SOCKET, SO_BROADCAST, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return;
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
				return;

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
				return;
			}

			// HACK: Only needed for the same system, which won't happen, unless we're testing
			m_winsockResult = setsockopt(m_commSocket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return;
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
				return;
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
}
#pragma endregion

#pragma region Updates
void Server::Update()
{
	// For each client
	for (m_mapIterator = m_mapOfClientAddrsAndTheirCommands.begin(); m_mapIterator != m_mapOfClientAddrsAndTheirCommands.end(); ++m_mapIterator)
	{
		// While client has commands
		while (m_mapIterator->second.IsEmpty() == false)
		{
			switch (m_mapIterator->second.Peek())
			{
			//case SharedNetwork::Command::GetNumber:
			//{
			//}
			//break;
			}
		}
	}

	// Sleep, just so it's not spinning super fast
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
#pragma endregion

#pragma region Public Functionality
bool Server::Join_StartRecvThread()
{
	return false;
}
void Server::RecvCommMessLoop()
{
	// Daemon thread
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
			default: // Right now, just default, but look into all that are required
			{
				// HACK: Make sure it's a disconnection first
				// If this came from a client within the map...
				if (m_mapOfClientAddrsAndTheirCommands.find(m_commSockAddrIn.sin_addr.S_un.S_addr) != m_mapOfClientAddrsAndTheirCommands.end())
				{
					RemoveClient(m_commSockAddrIn);
				}
			}
			break;
			}

			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
		}

		// Client command
		if (m_recvBuffer[0] == '#')
		{
			if (strcmp(m_recvBuffer, mr_sharedNetwork.COMMANDS[static_cast<int>(SharedNetwork::Command::GetNumber)]) == 0)
			{
				GenerateResponse(SharedNetwork::Response::SendNumber);

				SendCommMess();
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.COMMANDS[static_cast<int>(SharedNetwork::Command::Join)]) == 0)
			{
				constexpr int MAX_NUMBER_OF_CLIENTS = 4;
				if (m_numberOfConnectedClients < MAX_NUMBER_OF_CLIENTS)
				{
					m_mapOfClientAddrsAndTheirCommands.emplace(m_commSockAddrIn.sin_addr.S_un.S_addr, Queue<SharedNetwork::Command>());

					IncrementNumberOfConnectedClients();

					GenerateResponse(SharedNetwork::Response::Joined);

					SendCommMess();

					GenerateResponse(SharedNetwork::Response::SendNumber);

					SendCommMessToEveryClient();
				}
				else
				{
					GenerateResponse(SharedNetwork::Response::Full);

					SendCommMess();
				}
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.COMMANDS[static_cast<int>(SharedNetwork::Command::Leave)]) == 0)
			{
				RemoveClient(m_commSockAddrIn);
			}
		}

		// Other
		else
		{
			// m_mapOfClientAddrsAndTheirCommands.at(m_commSockAddrIn.sin_addr.S_un.S_addr).PushBack(SharedNetwork::Command::Join);
		}
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
			default: // Right now, just default, but look into all that are required
				break;
			}

			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
		}

		// Sleep
		std::this_thread::sleep_for(std::chrono::seconds(m_secondsBetweenBroadcast));
	}
}
#pragma endregion

#pragma region Private Functionality
void Server::GenerateResponse(SharedNetwork::Response _response)
{
	switch (_response)
	{
	case SharedNetwork::Response::Full:
	case SharedNetwork::Response::Joined:
		strcpy(m_sendBuffer, mr_sharedNetwork.RESPONSES[static_cast<int>(_response)]);
		break;
	case SharedNetwork::Response::SendNumber:
	{
		strcpy(m_sendBuffer, mr_sharedNetwork.RESPONSES[static_cast<int>(SharedNetwork::Response::SendNumber)]);
		strcat(m_sendBuffer, static_cast<const char*>(&mr_sharedNetwork.m_numOfConnClientsOnServ));
	}
	break;
	}
	// NOTE: Nightmare food! Converting from char to int
}
void Server::RemoveClient(const sockaddr_in& _commSockAddrIn)
{
	m_mapOfClientAddrsAndTheirCommands.erase(m_commSockAddrIn.sin_addr.S_un.S_addr);

	DecrementNumberOfConnectedClients();
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
		default: // Right now, just default, but look into all that are required
			break;
		}

		// Execution shouldn't make it here when everything is working properly
		throw std::exception();
	}
}
void Server::SendCommMessToEveryClient()
{
	for (m_mapIterator = m_mapOfClientAddrsAndTheirCommands.begin(); m_mapIterator != m_mapOfClientAddrsAndTheirCommands.end(); ++m_mapIterator)
	{
		m_commSockAddrIn.sin_addr.S_un.S_addr = m_mapIterator->first;

		SendCommMess();
	}
}
#pragma endregion