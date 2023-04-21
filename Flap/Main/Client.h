#ifndef CLIENT_H
#define CLIENT_H

#include "Host.h"

class SharedNetwork;

class Client final : public Host
{
public:
	// Initialization
	Client(SharedNetwork& _sharedNetwork);
	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;

	// Updates
	void Update();

	// Functionality
	bool Join_StartRecvThread() override;
	void RecvCommMessLoop() override;

	// Destruction
	inline ~Client() override { delete[] mr_sharedNetwork.mp_serverIPAddress; }

private:
	// Member Variables
	char* mp_walker_1;
	const char* mp_walker_2;
	static constexpr int CLIENT_COMMUNICATION_PORT = 8998;

	// Functionality
	bool CheckForSendNumber();
	void SendCommMess();
};

#endif CLIENT_H