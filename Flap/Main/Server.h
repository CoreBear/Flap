#ifndef SERVER_H
#define SERVER_H

#include "Host.h"
#include "Queue.h"

#include <unordered_map>

class SharedNetwork;

class Server final : public Host
{
public:
	// Initialization
	inline Server(SharedNetwork& _sharedNetwork) :
		Host(1, _sharedNetwork)							// HACK: Hardcoding
	{
		return;
	}
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	// Loops
	void UpdateLoop() override;

	// Functionality
	void Join() override;
	void RecvCommMessLoop() override;
	void Stop() override;

protected:
	// Functionality
#ifdef SAME_SYSTEM_NETWORK
	void AssignPort() override;
#endif SAME_SYSTEM_NETWORK
	void SendCommMess() override;

private:
	// Container
	struct MapVal
	{
	public:
		// Member Variables
		bool m_joined;
		int m_numberOfCyclesSinceLastPing;
		Queue<SharedNetwork::SpecialMessage> m_specialMessageQueue;

		// Initialization
		inline MapVal(bool _connected) :
			m_joined(_connected),
			m_numberOfCyclesSinceLastPing(0)
		{
			return;
		}
		MapVal() = default;
		MapVal(const MapVal&) = default;
		MapVal& operator=(const MapVal&) = delete;
	};

	// Member Variables
	int m_numberOfConnectedClients;
	int m_sizeofSockAddr;
	std::unordered_map<unsigned long, MapVal>m_mapOfClientAddrsConnTypeAndSpecMess;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapSendAllIterator;
	unsigned long m_sendingClientsAddrPort;

	// Functionality
	void AddrAndSendCommMess(unsigned long _addressOrPort);
	void HandleSpecMess();
	bool RemoveClient_EmptyMap(std::unordered_map<unsigned long, MapVal>::iterator& _iterator);
	void SendCommMessToEveryClient_Except(unsigned long _addressOrPort = ULONG_MAX);
};

#endif SERVER_H