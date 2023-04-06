#ifndef SHARED_GAME_H
#define SHARED_GAME_H

#include "Enums.h"

#include <mutex>

class SharedGame final
{
public:
	// Member Variables
	Enums::GameState m_gameState;
	std::mutex m_gameStateMutex;

	// Initialization
	inline SharedGame() : m_gameState(Enums::GameState::Menu) { return; }
	SharedGame(const SharedGame&) = delete;
	SharedGame& operator=(const SharedGame&) = delete;
};

#endif SHARED_GAME_H