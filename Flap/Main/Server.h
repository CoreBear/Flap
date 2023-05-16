#ifndef SERVER_H
#define SERVER_H

#include "Host.h"
#include "Queue.h"
#include "Structure.h"

#include <unordered_map>

class SharedGame;
class SharedNetwork;

class Server final : public Host
{
public:
	// Initialization
	inline Server(SharedGame& _sharedGame, SharedNetwork& _sharedNetwork) :
		Host(1, _sharedGame, _sharedNetwork),							// HACK: Hardcoding
		m_netState(NetState::InLobby)
	{
		mr_sharedNetwork.m_numOfConnClientsOnServChar = static_cast<char>(mr_sharedNetwork.m_numOfConnClientsOnServInt = 0) + '0';
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
	enum class NetState { InGame, InLobby, PreGame };
	struct MapVal
	{
	public:
		// Member Variables
		bool m_isReadyToPlay;
		bool m_joined;
		int m_numberOfCyclesSinceLastPing;
		Queue<SharedNetwork::SpecialMessage> m_specialMessageQueue;
		unsigned short m_networkPlayerIndex;
		Structure::Vector2<short> m_maxFrameBufferWidthHeight;

		// Initialization
		inline MapVal(bool _connected) :
			m_isReadyToPlay(false),
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
	int m_numOfClientsReadyToPlay;
	int m_playerNumberIndex;
	int m_sizeofSockAddr;
	NetState m_netState;
	std::unordered_map<unsigned long, MapVal>m_mapOfClientAddrsConnTypeAndSpecMess;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapJoinIterator;
	std::unordered_map<unsigned long, MapVal>::iterator m_mapSendAllIterator;
	unsigned long m_sendingClientsAddrPort;

	// Functionality
	void AddrAndSendCommMess(unsigned long _addressOrPort);
	void GenAndSendSetupMess();
	void HandleSpecMess();
	bool RemoveClient_EmptyMap(std::unordered_map<unsigned long, MapVal>::iterator& _iterator);
	void SendCommMessToEveryClient_Except(unsigned long _addressOrPort = ULONG_MAX);
};

#endif SERVER_H