#pragma region Includes
#include "NetworkManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>  
#pragma comment(lib,"Ws2_32.lib")

// NOTE/WARNING: Naming collision occurs when this stuff is above winsock.
// I believe it's coming from <windows.h>, but they're both integral to the system.

#include "Client.h"
#include "Consts.h"
#include "Host.h"
#include "Server.h"
#include "SharedGame.h"
#include "SharedNetwork.h"

#include <string>
#pragma endregion

#pragma region Initialization
NetworkManager::NetworkManager(SharedNetwork& _sharedNetwork) :
	mr_sharedNetwork(_sharedNetwork)
{
	// Initiates use of WS2_32.DLL by a processand must be called before any network functions!
	WSADATA wsadata;
	if (WSAStartup(WINSOCK_VERSION, &wsadata) != Consts::NO_VALUE) 
	{
		throw std::exception();
	}

	GenerateIPAddress();
}
#pragma endregion

#pragma region Public Functionality
void NetworkManager::Join()
{
	mp_host->Join();
}
void NetworkManager::RunHost(bool _isClient, SharedGame& _sharedGame, SharedInput& _sharedInput)
{
	mr_sharedNetwork.m_startNetworkedGameMutex.lock();
	mr_sharedNetwork.m_startNetworkedGame = false;
	mr_sharedNetwork.m_startNetworkedGameMutex.unlock();

	if (_isClient)
	{
		mp_host = new Client(_sharedGame, mr_sharedNetwork);

		mr_sharedNetwork.m_hostType = Enums::HostType::Client;
	}
	else
	{
		mp_host = new Server(_sharedGame, _sharedInput, mr_sharedNetwork);

		mr_sharedNetwork.m_hostType = Enums::HostType::Server;
	}

	mp_host->Init();

	m_updateLoopThread = std::thread(&Host::UpdateLoop, mp_host);

	std::thread(&Host::RecvCommMessLoop, mp_host).detach();
}
void NetworkManager::StopHost()
{
	mr_sharedNetwork.m_startNetworkedGameMutex.lock();
	mr_sharedNetwork.m_startNetworkedGame = false;
	mr_sharedNetwork.m_startNetworkedGameMutex.unlock();

	mp_host->Stop();

	m_updateLoopThread.join();

	delete mp_host;
	mp_host = nullptr;

	mr_sharedNetwork.m_hostType = Enums::HostType::NA;
}
#pragma endregion

#pragma region Private Functionality
void NetworkManager::GenerateIPAddress()
{
#ifdef TEST_ON_LOOP_BACK
	strcpy(mr_sharedNetwork.m_mayIPAddress, mr_sharedNetwork.LOOP_BACK_ADDR);
#else // !TEST_ON_LOOP_BACK
	// Get the actual name of this system
	char hostName[100];
	if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR)
	{
		throw std::exception();
	}

	// Get this system's address
	struct hostent* addresses = gethostbyname(hostName);
	if (addresses == 0)
	{
		throw std::exception();
	}

	// Store this system's address as a string
	{
		struct in_addr addr;
		memcpy(&addr, addresses->h_addr_list[0], sizeof(struct in_addr));

		const char* separator = ".";
		int octet = static_cast<int>(addr.S_un.S_un_b.s_b1);
		std::string string = std::to_string(octet);
		string.append(separator);
		octet = static_cast<int>(addr.S_un.S_un_b.s_b2);
		string += std::to_string(octet);
		string.append(separator);
		octet = static_cast<int>(addr.S_un.S_un_b.s_b3);
		string += std::to_string(octet);
		string.append(separator);
		octet = static_cast<int>(addr.S_un.S_un_b.s_b4);
		string += std::to_string(octet);

		for (size_t i = Consts::NO_VALUE; i < string.size(); i++)
		{
			mr_sharedNetwork.m_mayIPAddress[i] = string[i];
		}
	}
#endif TEST_ON_LOOP_BACK
}
#pragma endregion

#pragma region Destruction
NetworkManager::~NetworkManager()
{
	// Terminates use of the WS2_32.DLL
	WSACleanup();
}
#pragma endregion