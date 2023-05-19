#ifndef CLIENT_STATE_MACHINE_H
#define CLIENT_STATE_MACHINE_H

#include "Enums.h"
#include "Host.h"
#include "SharedNetwork.h"

#include <WinSock2.h>

class Client;
class SharedGame;

class ClientStateMachine
{
public:
	// Initialization
	ClientStateMachine(const ClientStateMachine&) = delete;
	ClientStateMachine& operator=(const ClientStateMachine&) = delete;
	inline virtual void Init() { return; }

	// Functionality
	inline virtual void Join() { return; }

	// Updates
	inline virtual void Update() { return; }

	// Destruction
	virtual ~ClientStateMachine() = default;
	inline virtual void Denit() { return; }

protected:
	// Member Variables
	Client* const mp_client;
	SharedNetwork& mr_sharedNetwork;

	// Initialization
	inline ClientStateMachine(Client& _client, SharedNetwork& _sharedNetwork) : 
		mp_client(&_client),
		mr_sharedNetwork(_sharedNetwork)
	{
		return;
	}
};
class AttemptingToJoinServer final : public ClientStateMachine
{
public:
	// Initialization
	inline AttemptingToJoinServer(Client& _client, SharedNetwork& _sharedNetwork) : ClientStateMachine(_client, _sharedNetwork) { return; }
	AttemptingToJoinServer(const AttemptingToJoinServer&) = delete;
	AttemptingToJoinServer& operator=(const AttemptingToJoinServer&) = delete;
	void Init() override;

private:
	// Member Variables
	int m_numberOfWaits;
};
class JoinedServer : public ClientStateMachine
{
public:
	// Initialization
	JoinedServer(const JoinedServer&) = delete;
	JoinedServer& operator=(const JoinedServer&) = delete;

	// Updates
	virtual void Update() override;

protected:
	// Initialization
	inline JoinedServer(Client& _client, SharedNetwork& _sharedNetwork) : ClientStateMachine(_client, _sharedNetwork) { return; }
};
#include "InputReceiver.h"
class JoinedServer_InGame final : public InputReceiver, public JoinedServer
{
public:
	// Member Variables
	char* mp_sendBuffer;
	int m_winsockErrno;
	int m_winsockResult;
	SharedGame* const mp_sharedGame;
	sockaddr_in& m_commSockAddrIn;
	SOCKET& m_commSocket;
	const unsigned short MAX_BUFF_SIZE;

	// Initialization
	inline JoinedServer_InGame(Client& _client, int _buffCap, SharedGame& _sharedGame, SharedNetwork& _sharedNetwork, sockaddr_in& _commSockAddrIn, SOCKET& _commSocket) :
		JoinedServer(_client, _sharedNetwork),
		mp_sendBuffer(new char[_buffCap]),
		m_winsockErrno(0),
		m_winsockResult(0),
		mp_sharedGame(&_sharedGame),
		m_commSockAddrIn(_commSockAddrIn),
		m_commSocket(_commSocket),
		MAX_BUFF_SIZE(_buffCap)
	{ 
		m_readIndex = 0;

		m_inputDirection = Enums::InputName::NA;
	}
	JoinedServer_InGame(const JoinedServer_InGame&) = delete;
	JoinedServer_InGame& operator=(const JoinedServer_InGame&) = delete;

	// Updates
	void Update() override;

	// Destruction
	inline ~JoinedServer_InGame() override { delete[] mp_sendBuffer; }

protected:
	// Functionality
	void InputDown(Enums::InputPressState _inputPressState) override;
	void InputLeft(Enums::InputPressState _inputPressState) override;
	void InputRight(Enums::InputPressState _inputPressState) override;
	void InputUp(Enums::InputPressState _inputPressState) override;

private:
	// Member Variables
	Enums::InputName m_inputDirection;
};
class JoinedServer_InLobby final : public JoinedServer
{
public:
	// Initialization
	inline JoinedServer_InLobby(Client& _client, SharedNetwork& _sharedNetwork) : JoinedServer(_client, _sharedNetwork) { return; }
	JoinedServer_InLobby(const JoinedServer_InLobby&) = delete;
	JoinedServer_InLobby& operator=(const JoinedServer_InLobby&) = delete;
};
class JoinedServer_PreGame final : public JoinedServer
{
public:
	// Initialization
	inline JoinedServer_PreGame(Client& _client, SharedNetwork& _sharedNetwork) : JoinedServer(_client, _sharedNetwork) { return; }
	JoinedServer_PreGame(const JoinedServer_PreGame&) = delete;
	JoinedServer_PreGame& operator=(const JoinedServer_PreGame&) = delete;
};
class NotJoined final : public ClientStateMachine
{
public:
	// Initialization
	inline NotJoined(Client& _client, SharedNetwork& _sharedNetwork) : ClientStateMachine(_client, _sharedNetwork) { return; }
	NotJoined(const NotJoined&) = delete;
	NotJoined& operator=(const NotJoined&) = delete;

	// Functionality
	void Join() override;
};
#endif CLIENT_STATE_MACHINE_H