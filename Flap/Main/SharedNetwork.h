#ifndef SHARED_NETWORK_H
#define SHARED_NETWORK_H

#include <mutex>

class SharedNetwork final
{
public:
	// Static Variables
	// NOTE: Non-actionable states will be handled by ServerSearchMenu.h
	enum class ClientState { AttemptToJoinServ, JoinedServ, NotJoined, NumberOfActionableStates, CouldNotConnect = NumberOfActionableStates, FullServ, InvalidAddr, ServDisc, NumberOfTotalStates };
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
	// Static Variables
	static constexpr int IP_ADDR_CHAR_LENGTH = 15;

	// Member Variables
	bool m_serverIPIsEmpty;
	bool m_serverDisconnected;
	bool m_waitForMenuUpdate;
	char m_numOfConnClientsOnServ;
	char* mp_myIPAddress;
	char m_serverIPAddress[IP_ADDR_CHAR_LENGTH]{ '_', '_', '_', '.', '_', '_', '_', '.', '_', '_', '_', '.', '_', '_', '_' };
	ClientState m_currentClientState;
	ClientState m_nextClientState;
	std::mutex m_nextClientStateMutex;
	std::mutex m_numOfConnClientsOnServMutex;
	std::mutex m_serverDisconnectedMutex;
	std::mutex m_serverIPAddressMutex;

	// Initialization
	inline SharedNetwork() :
		m_serverIPIsEmpty(true),
		m_serverDisconnected(false),
		m_waitForMenuUpdate(false),
		m_numOfConnClientsOnServ('0'),
		mp_myIPAddress(nullptr),
		m_currentClientState(ClientState::NotJoined),
		m_nextClientState(ClientState::NotJoined)
	{
		return;
	}
	SharedNetwork(const SharedNetwork&) = delete;
	SharedNetwork& operator=(const SharedNetwork&) = delete;
};

#endif SHARED_NETWORK_H