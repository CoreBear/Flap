#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <condition_variable>
#include <thread>

class Host;
class SharedNetwork;

class NetworkManager final
{
public:
	// Initialization
	NetworkManager(SharedNetwork& _sharedNetwork);
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;

	// Functionality
	void HostUpdate(bool _isClient);
	void Join();
	void RunHost(bool _isClient);
	void StopHost();

	// Destruction
	~NetworkManager();

private:
	// Member Variables
	bool m_killClient;
	std::condition_variable m_killClientConVar;
	Host* mp_host;
	SharedNetwork& mr_sharedNetwork;
	std::thread m_hostUpdateThread;
	std::thread m_updateLoopThread;

	// Functionality
	void GenerateIPAddress();
};

#endif NETWORK_MANAGER_H