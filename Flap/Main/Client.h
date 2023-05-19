#ifndef CLIENT_H
#define CLIENT_H

#include "Host.h"

class ClientStateMachine;
class SharedGame;
class SharedNetwork;

class Client final : public Host
{
public:
	// Initialization
	Client(SharedGame& _sharedGame, SharedNetwork& _sharedNetwork);
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
	static constexpr char NUM_OF_FRAMES_BETWEEN_UPDATE = 5;

	// Member Variables
	char m_recvChar;
	ClientStateMachine* m_currentClientState;
	ClientStateMachine** m_allClientStates;
	unsigned int m_updateTargetFrame;

	// Functionality
	void ForcedDisconnect();
	void SetNextState(SharedNetwork::ClientState _nextClientState, bool _waitForMenuUpdate = true);
};

#endif CLIENT_H