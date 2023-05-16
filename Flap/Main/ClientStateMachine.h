#ifndef CLIENT_STATE_MACHINE_H
#define CLIENT_STATE_MACHINE_H

#include "Host.h"
#include "SharedNetwork.h"

class Client;

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
	~ClientStateMachine() = default;
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

	// Updates
	void Update() override;

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
	void Update() override final;

protected:
	// Initialization
	inline JoinedServer(Client& _client, SharedNetwork& _sharedNetwork) : ClientStateMachine(_client, _sharedNetwork) { return; }
};
class JoinedServer_InGame : public JoinedServer
{
public:
	// Initialization
	inline JoinedServer_InGame(Client& _client, SharedNetwork& _sharedNetwork) : JoinedServer(_client, _sharedNetwork) { return; }
	JoinedServer_InGame(const JoinedServer_InGame&) = delete;
	JoinedServer_InGame& operator=(const JoinedServer_InGame&) = delete;
};
class JoinedServer_InLobby : public JoinedServer
{
public:
	// Initialization
	inline JoinedServer_InLobby(Client& _client, SharedNetwork& _sharedNetwork) : JoinedServer(_client, _sharedNetwork) { return; }
	JoinedServer_InLobby(const JoinedServer_InLobby&) = delete;
	JoinedServer_InLobby& operator=(const JoinedServer_InLobby&) = delete;
};
class JoinedServer_PreGame : public JoinedServer
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