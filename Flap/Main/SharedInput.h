#ifndef SHARED_INPUT_H
#define SHARED_INPUT_H

#include "Consts.h"
#include "Enums.h"
#include "Structure.h"

#include <mutex>
#include <queue>

class SharedInput final
{
public:
	// Initialization
	SharedInput();
	SharedInput(const SharedInput&) = delete;
	SharedInput& operator=(const SharedInput&) = delete;

	// Member Variables
	Enums::InputType m_inputType;
	std::mutex m_inputQueueMutex;
	std::mutex m_inputTypeMutex;
	std::queue<Structure::Input> m_inputQueue[Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM];
};

#endif SHARED_INPUT_H