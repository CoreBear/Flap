#pragma region Includes
#include "ClientStateMachine.h"

#include "Client.h"

#include <thread>
#pragma endregion

#pragma region Attempting To Join Server State
void AttemptingToJoinServer::Update()
{
	m_numberOfWaits = 0;

	// Wait for x-number of seconds, but wake up each second
	constexpr int MAX_NUMBER_OF_WAITS = 5;
	while (m_numberOfWaits < MAX_NUMBER_OF_WAITS)
	{
		// Wait
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_numberOfWaits++;

		// If client connected to server, return
		mr_sharedNetwork.m_nextClientStateMutex.lock();
		if (mr_sharedNetwork.m_nextClientState == SharedNetwork::ClientState::JoinedServ)
		{
			mr_sharedNetwork.m_nextClientStateMutex.unlock();
			return;
		}
		else
		{
			mr_sharedNetwork.m_nextClientStateMutex.unlock();
		}
	}

	// If execution makes it here, client did not connect to server
	mr_sharedNetwork.m_nextClientStateMutex.lock();
	mr_sharedNetwork.m_waitForMenuUpdate = true;
	mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::CouldNotConnect;
	mr_sharedNetwork.m_nextClientStateMutex.unlock();
}
#pragma endregion

#pragma region Joined Server State
#pragma region Updates
void JoinedServer::Update()
{
	mp_client->GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Ping);
}
#pragma endregion
#pragma endregion

#pragma region Looking For Server State
#pragma endregion

#pragma region Not Joined State
#pragma region Public Functionality
void NotJoined::Join()
{
#ifdef TEST_ON_LOOP_BACK
	// If attempting to connect to loopback
	if (strcmp(mr_sharedNetwork.m_serverIPAddress, "127.000.000.001") == 0)
#else !TEST_ON_LOOP_BACK
	// If attempting to connect to a valid address
	if (inet_addr(mr_sharedNetwork.m_serverIPAddress) != INADDR_NONE)
#endif TEST_ON_LOOP_BACK
	{
		mp_client->m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.m_serverIPAddress);	// Assign to server's address
		mp_client->m_commSockAddrIn.sin_port = htons(mp_client->SERVER_COMMUNICATION_PORT);					// Assign to server's port

		mp_client->GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Join);

		mr_sharedNetwork.m_nextClientStateMutex.lock();
		mr_sharedNetwork.m_waitForMenuUpdate = true;
		mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::AttemptToJoinServ;
		mr_sharedNetwork.m_nextClientStateMutex.unlock();
	}

	// If attempting to connect to an invalid address
	else
	{
		mr_sharedNetwork.m_nextClientStateMutex.lock();
		mr_sharedNetwork.m_waitForMenuUpdate = true;
		mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::InvalidAddr;
		mr_sharedNetwork.m_nextClientStateMutex.unlock();
	}
}
#pragma endregion
#pragma endregion