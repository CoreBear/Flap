#ifndef SHARED_NETWORK_H
#define SHARED_NETWORK_H

#include <mutex>

class SharedNetwork final
{
public:
	// Static Variables
	enum class SpecialMessage { Disconnect, Full, GetNumber, Join, Joined, Ping, SendNumber, NumberOfSpecialMessages};

	// Member Variables
	const char* const SPECIAL_MESSAGES[static_cast<int>(SpecialMessage::NumberOfSpecialMessages)] =
	{
		"#Disconnect",
		"#Full",
		"#GetNumber",
		"#Join",
		"#Joined",
		"#Ping",
		"#SendNumber"
	};

public:
	// Member Variables
	bool m_joinedServer;
	bool m_serverDisconnected;
	char m_numOfConnClientsOnServ;
	char* mp_myIPAddress;
	char* mp_serverIPAddress;
	std::mutex m_joinedServerMutex;
	std::mutex m_numOfConnClientsOnServMutex;
	std::mutex m_serverDisconnectedMutex;
	std::mutex m_serverIPAddressMutex;

	// Initialization
	inline SharedNetwork() :
		m_joinedServer(false),
		m_serverDisconnected(false),
		m_numOfConnClientsOnServ('0'),
		mp_myIPAddress(nullptr),
		mp_serverIPAddress(nullptr)
	{
		return;
	}
	SharedNetwork(const SharedNetwork&) = delete;
	SharedNetwork& operator=(const SharedNetwork&) = delete;
};

#endif SHARED_NETWORK_H