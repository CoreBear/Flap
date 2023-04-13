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
	inline bool GetIsInGameSession() const { return m_isInGameSession; }
	inline int GetNumberOfSnakesInGame() const { return m_numberOfSnakesInGame; }
	inline int& GetPlayerSnakeColorIndexRef(int _playerIndex) { return (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex : m_playerTwoSnakeColorIndex; }
	inline int GetSnakeStartingSpeed() const { return m_snakeStartingSpeed; }
	inline void IncrementNumberOfSnakesInGame() { ++m_numberOfSnakesInGame; }
	inline void SetPlayerSnakeColorIndex(int _colorIndex, int _playerIndex) { (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex = _colorIndex : m_playerTwoSnakeColorIndex = _colorIndex; }
	inline void ToggleIsInGameSession() { m_isInGameSession = !m_isInGameSession; }
	inline void ZeroNumberOfSnakesInGame() { m_numberOfSnakesInGame = Consts::NO_VALUE; }

private:
	bool m_isInGameSession;
	int m_numberOfSnakesInGame;
	int m_playerOneSnakeColorIndex;
	int m_playerTwoSnakeColorIndex;
	int m_snakeCurrentSpeed;
	int m_snakeStartingSpeed = 10;
};

#endif SHARED_GAME_H