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
	SharedGame();
	SharedGame(const SharedGame&) = delete;
	SharedGame& operator=(const SharedGame&) = delete;

	// Functionality
	inline void DecrementNumberOfPlayersInGame() { --m_numberOfPlayersInGame; }
	inline int GetNumberOfPlayersInGame() const { return m_numberOfPlayersInGame; }
	inline void SetNumberOfPlayersInGame(int _numberOfPlayersInGame) { m_numberOfPlayersInGame = _numberOfPlayersInGame; }

private:
	int m_numberOfPlayersInGame;
};

#endif SHARED_GAME_H