#ifndef CLIENT_H
#define CLIENT_H

#include "Host.h"

class ClientStateMachine;
class SharedNetwork;

class Client final : public Host
{
public:
	// Initialization
	Client(SharedNetwork& _sharedNetwork);
	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;

	// Loops
	void UpdateLoop() override;

	// Functionality
	void Join();
	void RecvCommMessLoop() override;
	void Stop() override;

	// Destruction
	~Client() override;

protected:
	// Functionality
#ifdef SAME_SYSTEM_NETWORK
	void AssignPort() override;
#endif SAME_SYSTEM_NETWORK
	void SendCommMess() override;

private:
	// Member Variables
	char* mp_walker_1;
	const char* mp_walker_2;
	ClientStateMachine* m_currentClientState;
	ClientStateMachine** m_allClientStates;

	// Functionality
	bool CheckForSendNumber();
	void ForcedDisconnect();
};

#endif CLIENT_H