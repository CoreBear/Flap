#pragma region Includes
#include "ClientStateMachine.h"

#include "Client.h"
#include "Consts.h"
#include "SharedGame.h"
#include "SharedNetwork.h"
#include "Tools.h"

#include <mutex>
#include <thread>
#pragma endregion

#pragma region Attempting To Join Server State
#pragma region Public Functionalit
void AttemptingToJoinServer::Init()
{
	m_numberOfWaits = Consts::NO_VALUE;

	// Wait for x-number of seconds, but wake up each second
	constexpr int MAX_NUMBER_OF_WAITS = 5;
	while (m_numberOfWaits < MAX_NUMBER_OF_WAITS)
	{
		// Wait
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_numberOfWaits++;

		// If client connected to server, return
		mr_sharedNetwork.m_nextClientStateMutex.lock();
		if (mr_sharedNetwork.m_nextClientState == SharedNetwork::ClientState::JoinedServ_InLobby)
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
#pragma endregion

#pragma region Joined Server State
#pragma region Updates
void JoinedServer::Update()
{
	mp_client->GenAssAndSendSpecMess(SharedNetwork::SpecialMessage::Ping);
}
void JoinedServer_InGame::Update()
{
	JoinedServer::Update();

	InputReceiver::HandleInput();

	if (m_inputDirection != Enums::InputName::NA)
	{
		// Example: "Input" - Add special message
		strcpy(mp_sendBuffer, mr_sharedNetwork.SPECIAL_MESSAGES[static_cast<int>(SharedNetwork::SpecialMessage::Input)]);

		const char* const PIPE_CHAR = "|";

		// Example: "Input|" - Add new pipe
		strcat(mp_sendBuffer, PIPE_CHAR);

		// Example: "Input|0" - Add input direction
		const char* intString = Tools::IntToString(static_cast<int>(m_inputDirection));
		strcat(mp_sendBuffer, intString);
		delete[] intString;

		m_winsockResult = sendto(m_commSocket, mp_sendBuffer, MAX_BUFF_SIZE, 0, (SOCKADDR*)&m_commSockAddrIn, sizeof(m_commSockAddrIn));
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

		m_inputDirection = Enums::InputName::NA;
	}
}
#pragma endregion

#pragma region Protected Functionality
void JoinedServer_InGame::InputDown(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Held:
	case Enums::InputPressState::PressedThisFrame:
		m_inputDirection = Enums::InputName::Down;
		break;
	}
}
void JoinedServer_InGame::InputLeft(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Held:
	case Enums::InputPressState::PressedThisFrame:
		m_inputDirection = Enums::InputName::Left;
		break;
	}
}
void JoinedServer_InGame::InputRight(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Held:
	case Enums::InputPressState::PressedThisFrame:
		m_inputDirection = Enums::InputName::Right;
		break;
	}
}
void JoinedServer_InGame::InputUp(Enums::InputPressState _inputPressState)
{
	switch (_inputPressState)
	{
	case Enums::InputPressState::Held:
	case Enums::InputPressState::PressedThisFrame:
		m_inputDirection = Enums::InputName::Up;
		break;
	}
}
#pragma endregion
#pragma endregion

#pragma region Not Joined State
#pragma region Public Functionality
void NotJoined::Join()
{
#ifdef TEST_ON_LOOP_BACK
	// If attempting to connect to loopback
	if (strcmp(mr_sharedNetwork.m_serverIPAddress, mr_sharedNetwork.LOOP_BACK_ADDR) == Consts::NO_VALUE)
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