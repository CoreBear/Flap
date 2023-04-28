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
class JoinedServer final : public ClientStateMachine
{
public:
	// Initialization
	inline JoinedServer(Client& _client, SharedNetwork& _sharedNetwork) : ClientStateMachine(_client, _sharedNetwork) { return; }
	JoinedServer(const JoinedServer&) = delete;
	JoinedServer& operator=(const JoinedServer&) = delete;

	// Updates
	void Update() override;
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