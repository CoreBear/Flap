#pragma region Includes
#include "SharedNetwork.h"

#include "Consts.h"
#pragma endregion

#pragma region Initialization
SharedNetwork::SharedNetwork() :
	m_serverIPIsEmpty(true),
	m_serverDisconnected(false),
	m_startNetworkedGame(false),
	m_waitForMenuUpdate(false),
	m_numOfConnClientsOnServChar('0'),
	m_currentClientState(ClientState::NotJoined),
	m_nextClientState(ClientState::NotJoined),
	m_hostType(Enums::HostType::NA),
	m_numOfConnClientsOnServInt(Consts::NO_VALUE)
{
	memset(m_mayIPAddress, 0, IP_ADDR_CHAR_LENGTH);
	m_mayIPAddress[IP_ADDR_CHAR_LENGTH] = '\0';
}
#pragma endregion