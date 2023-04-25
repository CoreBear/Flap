#pragma region Includes
#include "Host.h"
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