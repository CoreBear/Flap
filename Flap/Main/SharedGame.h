#ifndef SHARED_GAME_H
#define SHARED_GAME_H

#include "Consts.h"
#include "Enums.h"
#include "Structure.h"

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
	inline void DecrementNumberOfSnakesInGame() { --m_numberOfSnakesInGame; }
	inline int GetNumberOfSnakesInGame() const { return m_numberOfSnakesInGame; }
	inline void IncrementNumberOfSnakesInGame() { ++m_numberOfSnakesInGame; }
	inline void ZeroNumberOfSnakesInGame() { m_numberOfSnakesInGame = Consts::NO_VALUE; }

private:
	int m_numberOfSnakesInGame;
};

#endif SHARED_GAME_H