#pragma region Includes
#include "Client.h"
#pragma endregion

#pragma region Initialization
Client::Client(SharedNetwork& _sharedNetwork) : Host(_sharedNetwork)
{
	char sockOpt = '1';

	// Broadcast
	{
		SOCKET broadcastRecvSock;

		// Create broadcast receiving socket
		{
			broadcastRecvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (broadcastRecvSock == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();
				return;
			}

			// HACK: Only needed for the same system, which won't happen, unless we're testing
			m_winsockResult = setsockopt(broadcastRecvSock, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return;
			}
		}

		// Bind broadcast socket to receive
		{
			m_commSockAddrIn.sin_family = AF_INET;
			m_commSockAddrIn.sin_addr.S_un.S_addr = INADDR_ANY;				// 000's for all octets, only worried about the port
			m_commSockAddrIn.sin_port = htons(SERVER_BROADCAST_PORT);		// Listen for broadcast

			m_winsockResult = bind(broadcastRecvSock, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return;
			}
		}

		// Receive broadcast
		m_winsockResult = recvfrom(broadcastRecvSock, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
		if (m_winsockResult == SOCKET_ERROR)
		{
			m_winsockErrno = WSAGetLastError();
			return;
		}
	}

	// Communication
	{
		// Create socket
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


		// Bind socket to receive
		{
			// Parse receive buffer to get addr and port
			mp_walker_1 = m_recvBuffer;
			int stringIndex = 0;

			while (*mp_walker_1 != '|')
			{
				++mp_walker_1;
			}

			*mp_walker_1 = '\0';
			++mp_walker_1;

			m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.mp_myIPAddress);	// Bind to my address
			
			// HACK: Technically the client and server should be on the same port, but it cannot be done when both are localhost
			m_commSockAddrIn.sin_port = htons(CLIENT_COMMUNICATION_PORT);						// Bind to my port

			// Bind comm socket with server's port
			m_winsockResult = bind(m_commSocket, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return;
			}
		}

		// Update server addr info
		m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(m_recvBuffer);	// The server's address
		m_commSockAddrIn.sin_port = htons(atoi(mp_walker_1));					// The server's port
	
		// Store the server's address
		mr_sharedNetwork.m_serverIPAddressMutex.lock();
		mr_sharedNetwork.mp_serverIPAddress = new char[strlen(m_recvBuffer) + 1];
		strcpy(mr_sharedNetwork.mp_serverIPAddress, m_recvBuffer);
		mr_sharedNetwork.m_serverIPAddressMutex.unlock();
	}

	// Get connected number of users
	{
		strcpy(m_sendBuffer, mr_sharedNetwork.COMMANDS[static_cast<int>(SharedNetwork::Command::GetNumber)]);

		SendCommMess();

		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
		if (m_winsockResult == SOCKET_ERROR)
		{
			m_winsockErrno = WSAGetLastError();
			return;
		}

		// If proper response
		if (CheckForSendNumber())
		{
			// Store value in shared space
			mr_sharedNetwork.m_numOfConnClientsOnServMutext.lock();
			mr_sharedNetwork.m_numOfConnClientsOnServ = *mp_walker_1;
			mr_sharedNetwork.m_numOfConnClientsOnServMutext.unlock();
		}

		// If other response
		else
		{
			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
		}
	}
}
#pragma endregion

#pragma region Updates
void Client::Update()
{
	// Sleep, just so it's not spinning super fast
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
#pragma endregion

#pragma region Public Functionality
bool Client::Join_StartRecvThread()
{
	strcpy(m_sendBuffer, mr_sharedNetwork.COMMANDS[static_cast<int>(SharedNetwork::Command::Join)]);

	SendCommMess();

	m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
	if (m_winsockResult == SOCKET_ERROR)
	{
		m_winsockErrno = WSAGetLastError();
		return false;
	}

	// Display to the user via menu that the server was full
	if (strcmp(m_recvBuffer, mr_sharedNetwork.RESPONSES[static_cast<int>(SharedNetwork::Response::Full)]) == 0)
	{
		return false;
	}
	// Display to the user via menu that they joined
	else
	{
		return true;
	}
}
void Client::RecvCommMessLoop()
{
	// Daemon thread
	while (true)
	{
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
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

		// Server response
		if (m_recvBuffer[0] == '#')
		{
			if (CheckForSendNumber())
			{
				// Store value in shared space
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.lock();
				mr_sharedNetwork.m_numOfConnClientsOnServ = *mp_walker_1;
				mr_sharedNetwork.m_numOfConnClientsOnServMutext.unlock();
			}
		}

		// Other
		else
		{

		}
	}
}
#pragma endregion

#pragma region Protected Functionality
bool Client::CheckForSendNumber()
{
	mp_walker_1 = m_recvBuffer;
	mp_walker_2 = mr_sharedNetwork.RESPONSES[static_cast<int>(SharedNetwork::Response::SendNumber)];

	while (*mp_walker_2 != '\0')
	{
		// If any character is off, this is not the SendNumber response
		if (*mp_walker_1 != *mp_walker_2)
		{
			return false;
		}

		++mp_walker_1;
		++mp_walker_2;
	}

	return true;
}
void Client::SendCommMess()
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
//#pragma endregion