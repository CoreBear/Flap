#ifndef SERVER_H
#define SERVER_H

#include "Host.h"
#include "Queue.h"
#include "Structure.h"

#include <mutex>
#include <unordered_map>

class SharedGame;
class SharedInput;
class SharedNetwork;

class Server final : public Host
{
public:
	// Initialization
	Server(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedNetwork& _sharedNetwork);
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
		unsigned short m_networkPlayerIndex;
		Structure::Vector2<short> m_maxFrameBufferWidthHeight;

		// Initialization
		inline MapVal(bool _connected) :
			m_joined(_connected),
			m_numberOfCyclesSinceLastPing(0),
			m_networkPlayerIndex(0)
		{
			return;
		}
		MapVal() = default;
		MapVal(const MapVal&) = default;
		MapVal& operator=(const MapVal&) = delete;
	};

	// Member Variables
	bool m_isInGame;
	char m_colorBits;
	Structure::Input m_newInput;
	int m_sizeofSockAddr;
	SharedInput* const mp_sharedInput;
	std::unique_lock<std::mutex> m_frameBufferUniqueLock;
	std::unordered_map<unsigned long, MapVal>m_mapOfClientAddrsConnTypeAndSpecMess;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapJoinIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapSendAllIterator;
	unsigned long m_sendingClientsAddrPort;
	unsigned short m_networkPlayerIndex;

	// Functionality
	void AddrAndSendCommMess(unsigned long _addressOrPort);
	void HandleSpecMess();
	bool RemoveClient_EmptyMap(std::unordered_map<unsigned long, MapVal>::iterator& _iterator);
	void SendCommMessToEveryClient_Except(unsigned long _addressOrPort = ULONG_MAX);
	void StoreGameBoard();
};

#endif SERVER_H