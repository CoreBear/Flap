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

	// Static Variables
	static constexpr int LOW_HIGH_BIT_SHIFT = 4;
	static constexpr unsigned short MAX_BUFF_SIZE = 16080;

	// Containers
	//struct GameBoardMessage
	//{
	//public:
	//	// Member Variables
	//	bool* m_arr;

	//	// Initialization
	//	inline GameBoardMessage() : 
	//		m_arr(new bool[MAX_BUFF_SIZE * 8])	// Bool to char
	//	{
	//		return; 
	//	}
	//	GameBoardMessage(const GameBoardMessage&) = delete;
	//	GameBoardMessage& operator=(const GameBoardMessage&) = delete;

	//	// Destruction
	//	inline ~GameBoardMessage() { delete m_arr; }
	//} m_gameBoard;

	// Member Variables
	bool m_isRunning;
	char m_recvBuffer[MAX_BUFF_SIZE];
	char m_sendBuffer[MAX_BUFF_SIZE];
	char* mp_recvBuffWalkerLeading;
	char* mp_recvBuffWalkerTrailing;
	int m_cellIndex;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
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
	void AddNumFieldToNetString(int _number);
#ifdef SAME_SYSTEM_NETWORK
	virtual void AssignPort() = 0;
#endif SAME_SYSTEM_NETWORK
	bool CheckForSpecMess(SharedNetwork::SpecialMessage _specialMessage);
	void GenAssAndSendSpecMess(SharedNetwork::SpecialMessage _specialMessage, unsigned long _addressOrPort = ULONG_MAX);
	void GenSpecMess(SharedNetwork::SpecialMessage _specialMessage);
	void NextBufferString(bool _firstString);
	virtual void SendCommMess() = 0;
};

#endif HOST_H