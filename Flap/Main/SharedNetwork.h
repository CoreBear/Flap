#ifndef SHARED_NETWORK_H
#define SHARED_NETWORK_H

#include <mutex>

class SharedNetwork final
{
public:
	// Static Variables
	enum class Command { GetNumber, Join, Leave, NumberOfCommands };
	enum class Response { Full, Joined, SendNumber, NumberOfResponses };

	// Member Variables
	const char* const COMMANDS[static_cast<int>(Command::NumberOfCommands)] =
	{
		"#GetNumber",
		"#Join",
		"#Leave"
	};
	const char* const RESPONSES[static_cast<int>(Command::NumberOfCommands)] =
	{
		"#Full",
		"#Joined",
		"#SendNumber"
	};

public:
	// Member Variables
	char m_numOfConnClientsOnServ;
	char* mp_myIPAddress;
	char* mp_serverIPAddress;
	std::mutex m_numOfConnClientsOnServMutext;
	std::mutex m_serverIPAddressMutex;

	// Initialization
	inline SharedNetwork() :
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