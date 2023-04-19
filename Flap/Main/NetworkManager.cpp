#pragma region Includes
#include "NetworkManager.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS         // Turns off deprecated warnings for winsock

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>  
#pragma comment(lib,"Ws2_32.lib")

// NOTE/WARNING: Naming collision occurs when this is above winsock.
// I believe it's coming from <windows.h>, but they're both integral to the system.
#include "SharedGame.h"
#pragma endregion

#pragma region Initialization
NetworkManager::NetworkManager(SharedGame& _sharedGame) :
	m_run(true),
	mr_sharedGame(_sharedGame)
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
void NetworkManager::NetworkThreadEntry_Loop()
{
	while (m_run)
	{

	}
}
#pragma endregion

#pragma region Private Functionality
void NetworkManager::GenerateIPAddress()
{
	char hostName[100];
	if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR)
	{
		return;
	}

	struct hostent* addresses = gethostbyname(hostName);
	if (addresses == 0)
	{
		return;
	}

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

	mr_sharedGame.mp_ipAddress = new char[string.size() + 1];
	mr_sharedGame.mp_ipAddress[string.size()] = '\0';

	for (int i = 0; i < string.size(); i++)
	{
		mr_sharedGame.mp_ipAddress[i] = string[i];
	}

	//addrinfo hints;
	//addrinfo* result;

	//memset(&hints, 0, sizeof(hints));
	//hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	//hints.ai_socktype = 0;			/* Any type */
	//hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	//hints.ai_protocol = 0;          /* Any protocol */
	//hints.ai_canonname = NULL;
	//hints.ai_addr = NULL;
	//hints.ai_next = NULL;

	//getaddrinfo(hostName, NULL, &hints, &result);

	////freeaddrinfo
	////getnameinfo
	////gai_strerror
	//int h = 0;
}
#pragma endregion

#pragma region Destruction
NetworkManager::~NetworkManager()
{
	delete mr_sharedGame.mp_ipAddress;

	// Terminates use of the WS2_32.DLL
	WSACleanup();
}
#pragma endregion