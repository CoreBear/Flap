#ifndef HOST_H
#define HOST_H

#include "ClientStateMachine.h"
#include "Defines.h"
#include "SharedNetwork.h"

#include <WinSock2.h>

class SharedGame;

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
	char* mp_recvBuffWalker;
	const char* mp_specMessWalker;
	int m_winsockErrno;
	int m_winsockResult;
	SharedGame* const mp_sharedGame;
	SharedNetwork& mr_sharedNetwork;
	sockaddr_in m_commSockAddrIn;
	SOCKET m_commSocket;
	static constexpr unsigned short SERVER_BROADCAST_PORT = 3006;
	static constexpr unsigned short SERVER_COMMUNICATION_PORT = 9889;

	//Initialization
	inline Host(float m_numberOfSecondsBetweenFixedUpdate, SharedGame& _sharedGame, SharedNetwork& _sharedNetwork) :
		m_isRunning(true),
		m_winsockErrno(0),
		m_winsockResult(0),
		mp_sharedGame(&_sharedGame),
		mr_sharedNetwork(_sharedNetwork)
	{
		return;
	}

	// Functionality
#ifdef SAME_SYSTEM_NETWORK
	virtual void AssignPort() = 0;
#endif SAME_SYSTEM_NETWORK
	void GenAssAndSendSpecMess(SharedNetwork::SpecialMessage _specialMessage, unsigned long _addressOrPort = ULONG_MAX);
	void GenSpecMess(SharedNetwork::SpecialMessage _specialMessage);
	virtual void SendCommMess() = 0;
};

#endif HOST_H