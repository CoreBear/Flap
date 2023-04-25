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
	bool Init_Succeeded(bool& _killMe) override;
	inline Server(SharedNetwork& _sharedNetwork) :
		Host(1, _sharedNetwork),							// HACK: Hardcoding
		m_secondsBetweenBroadcast(NORMAL_BROADCAST_WAIT)
	{
		return;
	}
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	// Updates
	void UpdateLoop() override;

	// Functionality
	void Join() override;
	void RecvCommMessLoop() override;
	void Stop() override;

	// Destruction
	~Server() override;

protected:
	// Functionality
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
	char m_broadcastSendBuffer[UCHAR_MAX];
	const int NORMAL_BROADCAST_WAIT = 2;	// Needs to be above m_secondsBetweenBroadcast
	int m_numberOfConnectedClients;
	int m_secondsBetweenBroadcast;
	int m_sizeofSockAddr;
	const int INDEFINITE_BROADCAST_WAIT = UINT_MAX;
	sockaddr_in m_broadSockAddrIn;
	SOCKET m_broadSocket;
	std::unordered_map<unsigned long, MapVal>m_mapOfClientAddrsConnTypeAndSpecMess;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapSendAllIterator;

	// Functionality
	void HandleSpecMess();
	bool RemoveClient_EmptyMap(std::unordered_map<unsigned long, MapVal>::iterator& _iterator);
	void SendBroadMessLoop();
	void SendCommMessToEveryClient();
};

#endif SERVER_H