#ifndef CLIENT_H
#define CLIENT_H

#include "Host.h"

class SharedNetwork;

class Client final : public Host
{
public:
	// Initialization
	inline Client(SharedNetwork& _sharedNetwork) :
		Host(1, _sharedNetwork),						// HACK: Hardcoding
		m_attemptingOrConnectedToServer(false)
	{
		return;
	}
	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;
	bool Init_Succeeded(bool& _killMe) override;

	// Updates
	void UpdateLoop() override;

	// Functionality
	void Join();
	void RecvCommMessLoop() override;
	void Stop() override;

	// Destruction
	~Client() override;

protected:
	// Functionality
	void SendCommMess() override;

private:
	// Member Variables
	bool m_attemptingOrConnectedToServer;
	char* mp_walker_1;
	const char* mp_walker_2;

	// Functionality
	bool CheckForSendNumber();
	void ForcedDisconnect();
	void RecvCommMess();
};

#endif CLIENT_H