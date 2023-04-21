#ifndef HOST_H
#define HOST_H

#include "SharedNetwork.h"

#include <WinSock2.h>

class Host
{
public:
	// Initialization
	Host(const Host&) = delete;
	Host& operator=(const Host&) = delete;

	// Updates
	inline virtual void Update() = 0;

	// Functionality
	virtual bool Join_StartRecvThread() = 0;						// Join/start game for server. Join server for client.
	virtual void RecvCommMessLoop() = 0;
	virtual void SendBroadMessLoop() { return; }

	// Destruction
	virtual ~Host();

protected:
	// Member Variables
	char m_recvBuffer[UCHAR_MAX];
	char m_sendBuffer[UCHAR_MAX];
	int m_winsockErrno;
	int m_winsockResult;
	static constexpr int SERVER_BROADCAST_PORT = 3006;
	static constexpr int SERVER_COMMUNICATION_PORT = 9889;
	SharedNetwork& mr_sharedNetwork;
	sockaddr_in m_commSockAddrIn;
	SOCKET m_commSocket;

	//Initialization
	inline Host(SharedNetwork& _sharedNetwork) : mr_sharedNetwork(_sharedNetwork) { return; }
};

#endif HOST_H