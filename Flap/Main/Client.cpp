#pragma region Includes
#include "Client.h"

#include "BufferCell.h"
#include "ClientStateMachine.h"
#include "Consts.h"
#include "GameManager.h"
#include "SharedGame.h"
#include "SharedNetwork.h"

#include <random>
#pragma endregion

#pragma region Initialization
Client::Client(SharedGame& _sharedGame, SharedNetwork& _sharedNetwork) :
	Host(1, _sharedGame, _sharedNetwork),						// HACK: Hardcoding
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
		case SharedNetwork::ClientState::JoinedServ_InGame:
			m_allClientStates[stateIndex] = new JoinedServer_InGame(*this, MAX_BUFF_SIZE, _sharedGame, _sharedNetwork, m_commSockAddrIn, m_commSocket);
			break;
		case SharedNetwork::ClientState::JoinedServ_InLobby:
			m_allClientStates[stateIndex] = new JoinedServer_InLobby(*this, _sharedNetwork);
			break;
		case SharedNetwork::ClientState::JoinedServ_PreGame:
			m_allClientStates[stateIndex] = new JoinedServer_PreGame(*this, _sharedNetwork);
			break;
		case SharedNetwork::ClientState::NotJoined:
			m_currentClientState = m_allClientStates[stateIndex] = new NotJoined(*this, _sharedNetwork);
			break;
		}
	}

	mr_sharedNetwork.m_currentClientState = SharedNetwork::ClientState::NotJoined;	// Arbitrary state, as long as it's not NotJoined
	SetNextState(SharedNetwork::ClientState::NotJoined);

	m_updateTargetFrame = static_cast<unsigned int>(GameManager::s_masterFixedFrameCount + NUM_OF_FRAMES_BETWEEN_UPDATE);
}
#pragma endregion

#pragma region Loops
void Client::RecvCommMessLoop()
{
	// Daemon
	while (true)
	{
		m_winsockResult = recvfrom(m_commSocket, m_recvBuffer, MAX_BUFF_SIZE, Consts::NO_VALUE, NULL, NULL);
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
			if (CheckForSpecMess(SharedNetwork::SpecialMessage::SendNumber))
			{
				NextBufferString(false);

				// Store value in shared space
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.lock();
				mr_sharedNetwork.m_numOfConnClientsOnServChar = *mp_recvBuffWalkerTrailing;
				mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Disconnect)]) == Consts::NO_VALUE)
			{
				ForcedDisconnect();
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Full)]) == Consts::NO_VALUE)
			{
				SetNextState(SharedNetwork::ClientState::FullServ);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Joined)]) == Consts::NO_VALUE)
			{
				// Set server's address to send to
				m_commSockAddrIn.sin_addr.S_un.S_addr = inet_addr(mr_sharedNetwork.m_serverIPAddress);		

				SetNextState(SharedNetwork::ClientState::JoinedServ_InLobby);
			}
			else if (strcmp(m_recvBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::StartGame)]) == Consts::NO_VALUE)
			{
				mr_sharedNetwork.m_startNetworkedGameMutex.lock();
				mr_sharedNetwork.m_startNetworkedGame = true;
				mr_sharedNetwork.m_startNetworkedGameMutex.unlock();

				SetNextState(SharedNetwork::ClientState::JoinedServ_InGame, false);
			}
		}

		// Gameboard
		else
		{
			mp_sharedGame->m_frameBufferMutex.lock();

			mp_sharedGame->ResetFrameBuffer();

			m_cellIndex = Consts::NO_VALUE;

			for (m_reusableIterator_1 = Consts::NO_VALUE; m_reusableIterator_1 < mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_y; m_reusableIterator_1++)
			{
				for (m_reusableIterator_2 = Consts::NO_VALUE; m_reusableIterator_2 < mp_sharedGame->FRAME_BUFFER_HEIGHT_WIDTH.m_x; m_reusableIterator_2++)
				{
					// If NoTouchy
					if (m_recvBuffer[m_cellIndex] == CHAR_MAX)
					{
						mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_character = NO_TOUCHY_CHAR;
						mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_colorBFGround = Consts::BACKGROUND_COLORS[static_cast<int>(Enums::Color::Red)];
						m_cellIndex++;
						continue;
					}

					// First 4 bits (lower) are char (number in cell)
					// Store number from low bits
					m_recvChar = m_recvBuffer[m_cellIndex] & LOW_BIT_MASK;
					mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_character = (m_recvChar == NULL) ? Consts::EMPTY_SPACE_CHAR : m_recvChar + ASCII_CHAR_OFFSETTER;

					// Shift high bits to low bits
					m_recvBuffer[m_cellIndex] >>= LOW_HIGH_BIT_SHIFT;

					// Second 4 bits (higher) are color (background, foreground will be black)
					// Store high bits in a temp variable
					mp_sharedGame->mpp_frameBuffer[m_reusableIterator_1][m_reusableIterator_2].m_colorBFGround = Consts::BACKGROUND_COLORS[m_recvBuffer[m_cellIndex++]];
				}
			}

			mp_sharedGame->m_frameBufferMutex.unlock();
			mp_sharedGame->m_rendererConVar.notify_one();
		}
	}
}
void Client::UpdateLoop()
{
	while (m_isRunning)
	{
		if (GameManager::s_masterFixedFrameCount == m_updateTargetFrame)
		{
			m_updateTargetFrame = static_cast<unsigned int>(GameManager::s_masterFixedFrameCount + NUM_OF_FRAMES_BETWEEN_UPDATE);

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
			}

			// If client state did not change
			else
			{
				switch (mr_sharedNetwork.m_nextClientState)
				{
				case SharedNetwork::ClientState::CouldNotConnect:
				case SharedNetwork::ClientState::ServDisc:
				{
					mr_sharedNetwork.m_nextClientState = SharedNetwork::ClientState::NotJoined;
				}
				break;
				}
				mr_sharedNetwork.m_nextClientStateMutex.unlock();
			}
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
	m_winsockResult = sendto(m_commSocket, m_sendBuffer, MAX_BUFF_SIZE, 0, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
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
void Client::ForcedDisconnect()
{
	SetNextState(SharedNetwork::ClientState::ServDisc);

	mr_sharedNetwork.m_serverDisconnectedMutex.lock();
	mr_sharedNetwork.m_serverDisconnected = true;
	mr_sharedNetwork.m_serverDisconnectedMutex.unlock();
}
void Client::SetNextState(SharedNetwork::ClientState _nextClientState, bool _waitForMenuUpdate)
{
	mr_sharedNetwork.m_nextClientStateMutex.lock();
	mr_sharedNetwork.m_waitForMenuUpdate = _waitForMenuUpdate;
	mr_sharedNetwork.m_nextClientState = _nextClientState;
	mr_sharedNetwork.m_nextClientStateMutex.unlock();
}
#pragma endregion

#pragma region Destruction
Client::~Client()
{
	mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.lock();
	mr_sharedNetwork.m_numOfConnClientsOnServChar = '0';
	mr_sharedNetwork.m_numOfConnClientsOnServCharMutex.unlock();

	for (int stateIndex = Consts::NO_VALUE; stateIndex < static_cast<int>(SharedNetwork::ClientState::NumberOfActionableStates); stateIndex++)
	{
		delete m_allClientStates[stateIndex];
	}
	delete[] m_allClientStates;
}
#pragma endregion