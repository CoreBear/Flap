#ifndef HOST_H
#define HOST_H

#include "ClientStateMachine.h"
#include "SharedNetwork.h"

#include <WinSock2.h>

class Host
{
public:
	// Initialization
	Host(const Host&) = delete;
	Host& operator=(const Host&) = delete;
	void Init();

	// Loops
	virtual void RecvCommMessLoop() = 0;
	virtual void UpdateLoop() = 0;

	// Functionality
	virtual void Join() = 0;						// Join/start game for server. Join server for client.
	virtual void Stop() = 0;

	// Destruction
	virtual ~Host();

protected:
	// Friends
	friend class JoinedServer;
	friend class NotJoined;

	// Member Variables
	bool m_isRunning;
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
	inline Host(float m_numberOfSecondsBetweenFixedUpdate, SharedNetwork& _sharedNetwork) :
		m_isRunning(true),
		m_winsockErrno(0),
		m_winsockResult(0),
		mr_sharedNetwork(_sharedNetwork)
	{
		return;
	}

	// Functionality
#if SAME_SYSTEM_TESTING
	virtual void AssignPort() = 0;
#endif SAME_SYSTEM_TESTING
	void GenAssAndSendSpecMess(SharedNetwork::SpecialMessage _specialMessage, unsigned long _address = ULONG_MAX);
	void GenSpecMess(SharedNetwork::SpecialMessage _specialMessage);
	virtual void SendCommMess() = 0;
};

#endif HOST_H