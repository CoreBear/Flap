#pragma region Includes
#include "Client.h"

#include "ClientStateMachine.h"
#include "Consts.h"

#include <random>
#pragma endregion

#pragma region Initialization
Client::Client(SharedNetwork& _sharedNetwork) :
	Host(1, _sharedNetwork),						// HACK: Hardcoding
	m_currentClientState(nullptr),
	m_allClientStates(nullptr)
{
	m_allClientStates = new	ClientStateMachine*[static_cast<int>(SharedNetwork::SharedNetwork::ClientState::NumberOfActionableStates)];

	for (int stateIndex = Consts::NO_VALUE; stateIndex < static_cast<int>(SharedNetwork::ClientState::NumberOfActionableStates); stateIndex++)
	{
		switch (static_cast<SharedNetwork::ClientState>(stateIndex))
		{
		case SharedNetwork::ClientState::AttemptToJoinServ:
			m_allClientStates[stateIndex] = new AttemptingToJoinServer(*this, _sharedNetwork);
			break;
		case SharedNetwork::ClientState::JoinedServ:
			m_allClientStates[stateIndex] = new JoinedServer(*this, _sharedNetwork);
			break;
		case SharedNetwork::ClientState::NotJoined:
			m_currentClientState = m_allClientStates[stateIndex] = new NotJoined(*this, _sharedNetwork);
			break;
		}
	}

	mr_sharedNetwork.m_nextClientStateMutex.lock();
	mr_sharedNetwork.m_waitForMenuUpdate = true;
	mr_sharedNetwork.m_currentClientState = SharedNetwork::ClientState::NotJoined;	// Arbitrary state, as long as it's not NotJoined
	mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::NotJoined;
	mr_sharedNetwork.m_nextClientStateMutex.unlock();
}
#pragma endregion

#pragma region Loops
void Client::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, UCHAR_MAX, Consts::NO_VALUE, NULL, NULL);
		if (m_winsockResult == SOCKET_ERROR)
		{
			// HACK: Don't need the errno catch right here, just useful for debugging
			switch (m_winsockErrno = WSAGetLastError())
			{
			case WSAECONNRESET:		// This will be returned if the server hasn't binded to port yet. Make sure it doesn't have any other issue
			case WSAEINTR:			// This will be returned when client attempts disconnection
			case WSANOTINITIALISED:	// This will be returned when client attempts disconnection
			{
				int h = 0;
			}
				continue;
			case WSAENOTSOCK:		// This will be returned when client attempts disconnection
				return;
			// If disconnected from server (no matter which side does it)
			//{
			//	ForcedDisconnect();

			//	// Sleep, until the OS decides to clean this thread up
			//	std::this_thread::sleep_for(std::chrono::seconds(ULLONG_MAX));
			//}
			//return;
			default: // Right now, just default, but look into all that are required
				break;
			}

			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
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
				mr_sharedNetwork.m_nextClientStateMutex.lock();
				mr_sharedNetwork.m_waitForMenuUpdate = true;
				mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::FullServ;
				mr_sharedNetwork.m_nextClientStateMutex.unlock();
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Joined)]) == 0)
			{
				// Set server's address to send to
				m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.m_serverIPAddress);		

				mr_sharedNetwork.m_nextClientStateMutex.lock();
				mr_sharedNetwork.m_waitForMenuUpdate = true;
				mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::JoinedServ;
				mr_sharedNetwork.m_nextClientStateMutex.unlock();
			}
		}

		// Other
		else
		{

		}
	}
}
void Client::UpdateLoop()
{
	while (m_isRunning)
	{
		// If client state is actionable (i.e., it can be denit'd/init'd/updated) and it's not waiting for menu to update itself
		mr_sharedNetwork.m_nextClientStateMutex.lock();
		if (mr_sharedNetwork.m_nextClientState < SharedNetwork::ClientState::NumberOfActionableStates && mr_sharedNetwork.m_waitForMenuUpdate == false)
		{
			// If client state changed
			if (mr_sharedNetwork.m_currentClientState != mr_sharedNetwork.m_nextClientState)
			{
				// Change storage state while still locked
				mr_sharedNetwork.m_currentClientState = mr_sharedNetwork.m_nextClientState;

				mr_sharedNetwork.m_nextClientStateMutex.unlock();

				// Denitialize previous state
				if (m_currentClientState != nullptr)
				{
					m_currentClientState->Denit();
				}

				// Change to next state and initialize state
				m_currentClientState = m_allClientStates[static_cast<int>(mr_sharedNetwork.m_nextClientState)];
				m_currentClientState->Init();
			}

			// If client state did not change
			else
			{
				mr_sharedNetwork.m_nextClientStateMutex.unlock();
			}

			// Update the current state
			m_currentClientState->Update();

			// Sleep, so the client doesn't do this all too rapidly
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		// If client state did not change
		else
		{
			mr_sharedNetwork.m_nextClientStateMutex.unlock();
		}
	}
}
#pragma endregion

#pragma region Public Functionality
void Client::Join()
{
	m_currentClientState->Join();
}
void Client::Stop()
{
	// Stop host update
	m_isRunning = false;

	GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Disconnect);
}
#pragma endregion

#pragma region Protected Functionality
#ifdef SAME_SYSTEM_NETWORK
void Client::AssignPort()
{
	// HACK: Not the best of ideas, but this should rarely result in multiple clients having the same port number
	std::random_device random;
	m_commSockAddrIn.sin_port = htons(8998 + (random() % 100));	// Bind to my port (different port than server, because the client and server have the same IP Address)
}
#endif SAME_SYSTEM_NETWORK
void Client::SendCommMess()
{
	// NOTE: Container stores information about who it's being sent to
	m_winsockResult = sendto(m_commSocket, m_sendBuffer, UCHAR_MAX, 0, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
	if (m_winsockResult == SOCKET_ERROR)
	{
		// HACK: Don't need the errno catch right here, just useful for debugging
		switch (m_winsockErrno = WSAGetLastError())
		{
		case WSAEADDRNOTAVAIL:	// This is returned when trying to send to unbinded addr info
			break;
		default: // Right now, just default, but look into all that are required
		{
			// Execution shouldn't make it here when everything is working properly
			throw std::exception();
		}
		break;
		}
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

	mr_sharedNetwork.m_nextClientStateMutex.lock();
	mr_sharedNetwork.m_waitForMenuUpdate = true;
	mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::ServDisc;
	mr_sharedNetwork.m_nextClientStateMutex.unlock();

	mr_sharedNetwork.m_serverDisconnectedMutex.lock();
	mr_sharedNetwork.m_serverDisconnected = true;
	mr_sharedNetwork.m_serverDisconnectedMutex.unlock();
}
#pragma endregion

#pragma region Destruction
Client::~Client()
{
	mr_sharedNetwork.m_numOfConnClientsOnServMutex.lock();
	mr_sharedNetwork.m_numOfConnClientsOnServ = '0';
	mr_sharedNetwork.m_numOfConnClientsOnServMutex.unlock();

	for (int stateIndex = Consts::NO_VALUE; stateIndex < static_cast<int>(SharedNetwork::ClientState::NumberOfActionableStates); stateIndex++)
	{
		delete m_allClientStates[stateIndex];
	}
	delete[] m_allClientStates;
}
#pragma endregion