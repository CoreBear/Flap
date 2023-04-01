#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "Manager.h"

class NetworkManager final : public Manager
{
public:
	NetworkManager() = default;
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;
};

#endif NETWORK_MANAGER_H