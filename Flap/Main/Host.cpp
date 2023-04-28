#pragma region Includes
#include "Host.h"
#pragma endregion

#pragma region Initialization
void Host::Init()
{
	// Enable socket options
	char sockOpt = '1';

	// Communication
	{
		// Create socket
		{
			m_commSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (m_commSocket == INVALID_SOCKET)
			{
				m_winsockErrno = WSAGetLastError();

				// Execution shouldn't make it here when everything is working properly
				throw std::exception();
			}

#if SAME_SYSTEM_TESTING // Address can be reused for both the client and server(s)
			m_winsockResult = setsockopt(m_commSocket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();

				// Execution shouldn't make it here when everything is working properly
				throw std::exception();
			}
#endif SAME_SYSTEM_TESTING
		}

		// Bind socket to receive
		{
			m_commSockAddrIn.sin_family = AF_INET;
			m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.mp_myIPAddress);		// Bind to my address
			
#if SAME_SYSTEM_TESTING			// Ports need to be different, so hosts don't attempt to consume their own messages
			AssignPort();
#else !SAME_SYSTEM_TESTING		// Bind to common port
			m_commSockAddrIn.sin_port = htons(SERVER_COMMUNICATION_PORT);
#endif SAME_SYSTEM_TESTING

			m_winsockResult = bind(m_commSocket, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();

				// Execution shouldn't make it here when everything is working properly
				throw std::exception();
			}

			// 
			m_commSockAddrIn.sin_addr.S_un.S_addr = INADDR_ANY;
			m_commSockAddrIn.sin_port = 0;
		}
	}
}
#pragma endregion

#pragma region Protected Functionality
void Host::GenAssAndSendSpecMess(SharedNetwork::SpecialMessage _specialMessage, unsigned long _address)
{
	GenSpecMess(_specialMessage);

	// If an address comes in, assign it
	if (_address != ULONG_MAX)
	{
		m_commSockAddrIn.sin_addr.S_un.S_addr = _address;
	}

	SendCommMess();
}
void Host::GenSpecMess(SharedNetwork::SpecialMessage _specialMessage)
{
	// NOTE: This may only require an if/else or potentially most things can be dropped in a default
	switch (_specialMessage)
	{
	case SharedNetwork::SpecialMessage::Disconnect:
	case SharedNetwork::SpecialMessage::Full:
	case SharedNetwork::SpecialMessage::GetNumber:
	case SharedNetwork::SpecialMessage::Join:
	case SharedNetwork::SpecialMessage::Joined:
	case SharedNetwork::SpecialMessage::Ping:
		strcpy(m_sendBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(_specialMessage)]);
		break;
	case SharedNetwork::SpecialMessage::SendNumber:
	{
		strcpy(m_sendBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::SendNumber)]);
		strcat(m_sendBuffer, static_cast<const char*>(&mr_sharedNetwork.m_numOfConnClientsOnServ));
	}
	break;
	}
}
#pragma endregion

#pragma region Destruction
Host::~Host() 
{
	// Does nothing, since this id UDP
	shutdown(m_commSocket, SD_BOTH);

	closesocket(m_commSocket);
}
#pragma endregion