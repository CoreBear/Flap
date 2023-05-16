#ifndef SHARED_NETWORK_H
#define SHARED_NETWORK_H

#include "Enums.h"

#include <mutex>

class SharedNetwork final
{
public:
	// Static Variables
	// NOTE: Non-actionable states will be handled by ServerSearchMenu.h
	enum class ClientState { AttemptToJoinServ, JoinedServ_InGame, JoinedServ_InLobby, JoinedServ_PreGame, NotJoined, NumberOfActionableStates, CouldNotConnect = NumberOfActionableStates, FullServ, InvalidAddr, ServDisc, NumberOfTotalStates };
	enum class SpecialMessage { ClientReady, Disconnect, Full, GetNumber, Join, Joined, Ping, SendNumber, Setup, SetupComplete, NumberOfSpecialMessages};

	// Member Variables
	const char* const SPECIAL_MESSAGES[static_cast<int>(SpecialMessage::NumberOfSpecialMessages)] =
	{
		"#ClientReady",
		"#Disc",
		"#Full",
		"#GetN",
		"#Join",
		"#Joined",
		"#Ping",
		"#SendN",
		"#Setup",
		"#SetupComplete"
	};

public:
	// Static Variables
	static constexpr int IP_ADDR_CHAR_LENGTH = 15;

	// Member Variables
	bool m_serverIPIsEmpty;
	bool m_serverDisconnected;
	bool m_startNetworkedGame;
	bool m_waitForMenuUpdate;
	char m_numOfConnClientsOnServChar;
	char m_mayIPAddress[IP_ADDR_CHAR_LENGTH + 1];
	char m_serverIPAddress[IP_ADDR_CHAR_LENGTH + 1]{ '_', '_', '_', '.', '_', '_', '_', '.', '_', '_', '_', '.', '_', '_', '_', '\0' };
	const char* const LOOP_BACK_ADDR = "127.000.000.001";
	ClientState m_currentClientState;
	ClientState m_nextClientState;
	Enums::HostType m_hostType;
	int m_mostRecentClientInput;
	int m_numOfConnClientsOnServInt;
	std::mutex m_mostRecentClientInputMutex;
	std::mutex m_nextClientStateMutex;
	std::mutex m_numOfConnClientsOnServCharMutex;
	std::mutex m_serverDisconnectedMutex;
	std::mutex m_serverIPAddressMutex;
	std::mutex m_startNetworkedGameMutex;

	// Initialization
	SharedNetwork();
	SharedNetwork(const SharedNetwork&) = delete;
	SharedNetwork& operator=(const SharedNetwork&) = delete;
};

#endif SHARED_NETWORK_H