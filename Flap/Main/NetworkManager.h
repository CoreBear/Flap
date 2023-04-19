#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

class SharedGame;

class NetworkManager final
{
public:
	// Initialization
	NetworkManager(SharedGame& _sharedGame);
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;

	// Functionality
	void InitHost(bool _isServer);
	void NetworkThreadEntry_Loop();
	inline void StopNetworking() { m_run = false; }

	// Destruction
	~NetworkManager();

private:
	// Member Variables
	bool m_run;
	SharedGame& mr_sharedGame;

	// Functionality
	void GenerateIPAddress();
};

#endif NETWORK_MANAGER_H