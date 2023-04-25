#pragma region Includes
#include "Client.h"
#pragma endregion

#pragma region Initialization
bool Client::Init_Succeeded(bool& _killMe)
{
	// Enable socket options
	char sockOpt = '1';

	// Broadcast
	{
		SOCKET m_broadcastSocket;

		// Create broadcast receiving socket
		{
			m_broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (m_broadcastSocket == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}

			// HACK: Only needed for the same system, which won't happen, unless we're testing
			m_winsockResult = setsockopt(m_broadcastSocket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}
		}

		// Bind broadcast socket to receive
		{
			m_commSockAddrIn.sin_family = AF_INET;
			m_commSockAddrIn.sin_addr.S_un.S_addr = INADDR_ANY;				// 000's for all octets, only worried about the port
			m_commSockAddrIn.sin_port = htons(SERVER_BROADCAST_PORT);		// Listen for broadcast

			m_winsockResult = bind(m_broadcastSocket, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
			}
		}

		// Receive broadcast
		m_winsockResult = recvfrom(m_broadcastSocket, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
		if (m_winsockResult == SOCKET_ERROR)
		{
			m_winsockErrno = WSAGetLastError();
			return false;
		}
	}

	if (_killMe)
	{
		return false;
	}
	
	// Communication
	{
		// Create socket
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

			// If client and server have the same IP
			if (strcmp(mr_sharedNetwork.mp_myIPAddress, m_recvBuffer) == 0)
			{
				m_commSockAddrIn.sin_port = htons(8998);	// Bind to my port (different port than server, because the client and server have the same IP Address)
			}

			// If client and server have different IP's
			else
			{
				m_commSockAddrIn.sin_port = htons(SERVER_COMMUNICATION_PORT);	// Bind to my port (same port as server)
			}

			// Bind comm socket with server's port
			m_winsockResult = bind(m_commSocket, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();
				return false;
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
		GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::GetNumber);

		// Break for specific circumstances
		/*while (true)
		{*/
			RecvCommMess();

			/*switch (switch_on)
			{
			case:
				break;
			case:
				break;
			case:
				break;
			case:
				return true;
			}
		}*/
	}
}
#pragma endregion

#pragma region Updates
void Client::UpdateLoop()
{
	while (m_isRunning)
	{
		GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Ping);

		// So the client doesn't do this all too rapidly
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
#pragma endregion

#pragma region Public Functionality
void Client::Join()
{
	if (m_attemptingOrConnectedToServer == false)
	{
		m_attemptingOrConnectedToServer = true;

		GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Join);
	}
}
void Client::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		RecvCommMess();
	}
}
void Client::Stop()
{
	// Stop host update
	m_isRunning = false;

	GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Disconnect);
}
#pragma endregion

#pragma region Protected Functionality
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
		//throw std::exception("user generated");
	}
}
#pragma endregion

#pragma region Private Functionality
bool Client::CheckForSendNumber()
{
	mp_walker_1 = m_recvBuffer;
	mp_walker_2 = mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::SendNumber)];

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
void Client::ForcedDisconnect()
{
	// Stop host update
	m_isRunning = false;

	// Notify the user that the server disconnected

	mr_sharedNetwork.m_serverDisconnectedMutex.lock();
	mr_sharedNetwork.m_serverDisconnected = true;
	mr_sharedNetwork.m_serverDisconnectedMutex.unlock();
}
void Client::RecvCommMess()
{
	m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, 0, NULL, NULL);
	if (m_winsockResult == SOCKET_ERROR)
	{
		// HACK: Don't need the errno catch right here, just useful for debugging
		switch (m_winsockErrno = WSAGetLastError())
		{
			// If disconnected from server (no matter which side does it)
		case WSAECONNRESET:
		case WSAEINTR:
		case WSAENOTSOCK:
		case WSANOTINITIALISED:
		{
			ForcedDisconnect();
			
			// Sleep, until the OS decides to clean this thread up
			std::this_thread::sleep_for(std::chrono::seconds(ULLONG_MAX));
		}
		return;
		default: // Right now, just default, but look into all that are required
			break;
		}

		// Execution shouldn't make it here when everything is working properly
		throw std::exception("user generated");
	}

	// Server response
	if (m_recvBuffer[0] == '#')
	{
		if (CheckForSendNumber())
		{
			// Store value in shared space
			mr_sharedNetwork.m_numOfConnClientsOnServMutex.lock();
			mr_sharedNetwork.m_numOfConnClientsOnServ = *mp_walker_1;
			mr_sharedNetwork.m_numOfConnClientsOnServMutex.unlock();
		}
		else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Disconnect)]) == 0)
		{
			ForcedDisconnect();
		}
		else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Full)]) == 0)
		{
			m_attemptingOrConnectedToServer = false;

			// Display to the user that the server was full
		}
		else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Joined)]) == 0)
		{
			// Display to the user that they've joined the server

			mr_sharedNetwork.m_joinedServerMutex.lock();
			mr_sharedNetwork.m_joinedServer = true;
			mr_sharedNetwork.m_joinedServerMutex.unlock();
		}
	}

	// Other
	else
	{

	}
}
#pragma endregion

#pragma region Destruction
Client::~Client()
{
	mr_sharedNetwork.m_joinedServerMutex.lock();
	mr_sharedNetwork.m_joinedServer = false;
	mr_sharedNetwork.m_joinedServerMutex.unlock();

	mr_sharedNetwork.m_numOfConnClientsOnServMutex.lock();
	mr_sharedNetwork.m_numOfConnClientsOnServ = '0';
	mr_sharedNetwork.m_numOfConnClientsOnServMutex.unlock();

	mr_sharedNetwork.m_serverIPAddressMutex.lock();
	delete[] mr_sharedNetwork.mp_serverIPAddress;
	mr_sharedNetwork.mp_serverIPAddress = nullptr;
	mr_sharedNetwork.m_serverIPAddressMutex.unlock();
}
#pragma endregion