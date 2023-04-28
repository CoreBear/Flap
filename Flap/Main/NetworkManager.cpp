#pragma region Includes
#include "NetworkManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>  
#pragma comment(lib,"Ws2_32.lib")

// NOTE/WARNING: Naming collision occurs when this stuff is above winsock.
// I believe it's coming from <windows.h>, but they're both integral to the system.

#include "Client.h"
#include "Host.h"
#include "Server.h"
#include "SharedNetwork.h"

#include <string>
#pragma endregion

#pragma region Initialization
NetworkManager::NetworkManager(SharedNetwork& _sharedNetwork) :
	mr_sharedNetwork(_sharedNetwork)
{
	// Initiates use of WS2_32.DLL by a processand must be called before any network functions!
	WSADATA wsadata;
	if (WSAStartup(WINSOCK_VERSION, &wsadata) != 0) 
	{
		return;
	}

	GenerateIPAddress();
}
#pragma endregion

#pragma region Public Functionality
void NetworkManager::Join()
{
	mp_host->Join();
}
void NetworkManager::RunHost(bool _isClient)
{
	if (_isClient)
	{
		mp_host = new Client(mr_sharedNetwork);
	}
	else
	{
		mp_host = new Server(mr_sharedNetwork);
	}

	mp_host->Init();

	m_updateLoopThread = std::thread(&Host::UpdateLoop, mp_host);

	std::thread(&Host::RecvCommMessLoop, mp_host).detach();
}
void NetworkManager::StopHost()
{
	mp_host->Stop();

	m_updateLoopThread.join();

	delete mp_host;
	mp_host = nullptr;
}
#pragma endregion

#pragma region Private Functionality
void NetworkManager::GenerateIPAddress()
{
	// Get the actual name of this system
	char hostName[100];
	if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR)
	{
		return;
	}

	// Get this system's address
	struct hostent* addresses = gethostbyname(hostName);
	if (addresses == 0)
	{
		return;
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

		mr_sharedNetwork.mp_myIPAddress = new char[string.size() + 1];
		mr_sharedNetwork.mp_myIPAddress[string.size()] = '\0';

		for (int i = 0; i < string.size(); i++)
		{
			mr_sharedNetwork.mp_myIPAddress[i] = string[i];
		}
	}
}
#pragma endregion

#pragma region Destruction
NetworkManager::~NetworkManager()
{
	delete mr_sharedNetwork.mp_myIPAddress;

	// Terminates use of the WS2_32.DLL
	WSACleanup();
}
#pragma endregion