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
	Server(SharedNetwork& _sharedNetwork);
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;

	// Updates
	void Update();

	// Functionality
	bool Join_StartRecvThread() override;
	void RecvCommMessLoop() override;
	void SendBroadMessLoop() override;

private:
	// Member Variables
	char m_broadcastSendBuffer[UCHAR_MAX];
	const int NORMAL_BROADCAST_WAIT = 2;	// Needs to be above m_secondsBetweenBroadcast
	int m_numberOfConnectedClients;
	int m_secondsBetweenBroadcast;
	int m_sizeofSockAddr;
	const int INDEFINITE_BROADCAST_WAIT = ULLONG_MAX;
	sockaddr_in m_broadSockAddrIn;
	SOCKET m_broadSocket;
	std::unordered_map<unsigned long, Queue<SharedNetwork::Command>> m_mapOfClientAddrsAndTheirCommands;
	std::unordered_map<unsigned long, Queue<SharedNetwork::Command>>::iterator m_mapIterator;

	// Functionality
	inline void DecrementNumberOfConnectedClients() { mr_sharedNetwork.m_numOfConnClientsOnServ = static_cast<char>(--m_numberOfConnectedClients) + '0'; }
	void GenerateResponse(SharedNetwork::Response _response);
	inline void IncrementNumberOfConnectedClients() { mr_sharedNetwork.m_numOfConnClientsOnServ = static_cast<char>(++m_numberOfConnectedClients) + '0'; }
	void RemoveClient(const sockaddr_in& _commSockAddrIn);
	void SendCommMess();
	void SendCommMessToEveryClient();
};

#endif SERVER_H