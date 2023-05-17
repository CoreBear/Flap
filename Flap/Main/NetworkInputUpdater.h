#ifndef NETWORK_INPUT_UPDATER
#define NETWORK_INPUT_UPDATER

#include "Structure.h"

class SharedInput;
class SharedNetwork;

class NetworkInputUpdater
{
public:
	// Initialization
	NetworkInputUpdater(SharedInput& _sharedInput, SharedNetwork& _sharedNetwork) : 
		mr_sharedInput(_sharedInput),
		mr_sharedNetwork(_sharedNetwork)
	{
		return; 
	}
	NetworkInputUpdater(const NetworkInputUpdater&) = delete;
	NetworkInputUpdater& operator=(const NetworkInputUpdater&) = delete;

	// Updates
	void Update();

private:
	// Member Variables
	Structure::Input m_currentInput;
	SharedInput& mr_sharedInput;
	SharedNetwork& mr_sharedNetwork;
};

#endif NETWORK_INPUT_UPDATER