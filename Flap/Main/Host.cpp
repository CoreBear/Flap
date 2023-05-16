#pragma region Includes
#include "Host.h"

#include "Consts.h"
#include "SharedGame.h"
#include "Tools.h"
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

#ifdef SAME_SYSTEM_NETWORK // Address can be reused for both the client and server(s)
			m_winsockResult = setsockopt(m_commSocket, SOL_SOCKET, SO_REUSEADDR, &sockOpt, sizeof(sockOpt));
			if (m_winsockResult == SOCKET_ERROR)
			{
				m_winsockErrno = WSAGetLastError();

				// Execution shouldn't make it here when everything is working properly
				throw std::exception();
			}
#endif SAME_SYSTEM_NETWORK
		}

		// Bind socket to receive
		{
			m_commSockAddrIn.sin_family = AF_INET;
			m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.m_mayIPAddress);		// Bind to my address
			
#ifdef SAME_SYSTEM_NETWORK			// Ports need to be different, so hosts don't attempt to consume their own messages
			AssignPort();
#else !SAME_SYSTEM_NETWORK		// Bind to common port
			m_commSockAddrIn.sin_port = htons(SERVER_COMMUNICATION_PORT);
#endif SAME_SYSTEM_NETWORK

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
void Host::AddNumFieldToNetString(int _number)
{
	const char* const PIPE_CHAR = "|";

	// Add new pipe
	strcat(m_sendBuffer, PIPE_CHAR);

	const char* intString = Tools::IntToString(_number);
	strcat(m_sendBuffer, intString);
	delete[] intString;
}
bool Host::CheckForSpecMess(SharedNetwork::SpecialMessage _specialMessage)
{
	NextBufferString(true);

	// Return if special message or not
	return (strcmp(mp_recvBuffWalkerTrailing, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(_specialMessage)]) == Consts::NO_VALUE);
}
void Host::GenAssAndSendSpecMess(SharedNetwork::SpecialMessage _specialMessage, unsigned long _addressOrPort)
{
	GenSpecMess(_specialMessage);

	// If an address comes in, assign it
	if (_addressOrPort != ULONG_MAX)
	{
#ifdef SAME_SYSTEM_NETWORK
		m_commSockAddrIn.sin_port = static_cast<unsigned short>(_addressOrPort);
#else !SAME_SYSTEM_NETWORK
		m_commSockAddrIn.sin_addr.S_un.S_addr = _addressOrPort;
#endif SAME_SYSTEM_NETWORK
	}

	SendCommMess();
}
void Host::GenSpecMess(SharedNetwork::SpecialMessage _specialMessage)
{
	// "WhateverTheSpecMess" - Add special
	strcpy(m_sendBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(_specialMessage)]);

	// NOTE: This may only require an if/else or potentially most things can be dropped in a default
	switch (_specialMessage)
	{
		// This client's playable screen dimensions (width|height)
	case SharedNetwork::SpecialMessage::Join:
	{
		// Example: "Join|240" - Add pipe and game area width
		AddNumFieldToNetString(mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_x);

		// Example: "Join|240|70" - Add pipe and game area height
		AddNumFieldToNetString(mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_y);
	}
	break;

	// Number of clients connected
	// Example Message: "#SendNumber|123456"
	case SharedNetwork::SpecialMessage::SendNumber:
	{
		strcat(m_sendBuffer, "|");
		strcat(m_sendBuffer, static_cast<const char*>(&mr_sharedNetwork.m_numOfConnClientsOnServChar));
	}
	break;

	default:
		break;
	}
}
void Host::NextBufferString(bool _firstString)
{
	if (_firstString)
	{
		mp_recvBuffWalkerLeading = m_recvBuffer;
		mp_recvBuffWalkerTrailing = m_recvBuffer;
	}
	else
	{
		mp_recvBuffWalkerTrailing = ++mp_recvBuffWalkerLeading;
	}

	// Move to next pipe
	while (*mp_recvBuffWalkerLeading != '|')
	{
		// If end of string, stop
		if (*mp_recvBuffWalkerLeading == '\0')
		{
			return;
		}

		++mp_recvBuffWalkerLeading;
	}

	// Null pipe
	*mp_recvBuffWalkerLeading = '\0';

	return;
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