#ifndef SHARED_GAME_H
#define SHARED_GAME_H

#include "Consts.h"
#include "Enums.h"
#include "Queue.h"
#include "Structure.h"

#include <mutex>
#include <queue>
#include <random>

class SharedGame final
{
public:
	// Static Variables
	static constexpr int MAX_NUMBER_OF_INITIALS = 3;
	static constexpr int MAX_HS_STRING_LENGTH = 50;			// NOTE: Arbitrary value

	// Member Variables
	char m_initials[MAX_NUMBER_OF_INITIALS]{ 'C','L','M' };
	char** mpp_highScoreLines;
	Enums::GameState m_gameState;
	int m_largestSnakeLengthUponDeath;
	int m_numberOfFramesBetweenSpawn;
	const int MAX_NUMBER_OF_NODES_TO_ADD;
	static constexpr int MAX_NUMBER_OF_HIGH_SCORES = 10;	// NOTE: Arbitrary value
	std::mutex m_gameStateMutex;
	Queue<int>* mp_availableSpawnPositions;
	std::random_device m_random;
	unsigned short m_largestSnakeColor;
	Structure::Vector2 m_currentPlayAreaSizeDimensions;
	const Structure::Vector2 MAX_WINDOW_SIZE_Dimensions;

	// Initialization
	SharedGame(const Structure::Vector2& _maxWindowSizeDimensions);
	SharedGame(const SharedGame&) = delete;
	SharedGame& operator=(const SharedGame&) = delete;

	// Functionality
	void AddAvailableSpawnIndex(int _x, int _y);
	inline void DecrementNumberOfSnakesInGame() { --m_numberOfSnakesInGame; }
	void GameSession(bool _isInGameSession, bool _isSinglePlayerGame);
	inline bool GetIsInGameSession() const { return m_isInGameSession; }
	inline int GetNumberOfFramesBeforeGameStart() const { return m_numberOfFramesBeforeGameStart; }
	inline int GetNumberOfSnakesInGame() const { return m_numberOfSnakesInGame; }
	inline int& GetPlayerSnakeColorIndexRef(int _playerIndex) { return (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex : m_playerTwoSnakeColorIndex; }
	const Structure::Vector2& GetRandomSpawnPositionRef();
	inline bool GetSinglePlayerBool() const { return m_isSinglePlayerGame; }
	inline int GetSnakeSpeed() const { return m_snakeCurrentSpeed; }
	const Structure::Vector2& GetSnakeStartPositionRef(int _numberOfPlayersIndex, int _playerIndex);
	inline void IncrementNumberOfSnakesInGame() { ++m_numberOfSnakesInGame; }
	void RemoveAvailableSpawnIndex(int _x, int _y);
	void ResetAvailableSpawnIndices();
	inline void ResetSnakeSpeed() { m_snakeCurrentSpeed = m_snakeStartingSpeed; }
	inline void SetSinglePlayerBool(bool _isSinglePlayerGame) { m_isSinglePlayerGame = _isSinglePlayerGame; }
	inline void SetPlayerSnakeColorIndex(int _colorIndex, int _playerIndex) { (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex = _colorIndex : m_playerTwoSnakeColorIndex = _colorIndex; }
	inline void SetSnakeSpeed(int _snakeSpeed) { m_snakeCurrentSpeed = _snakeSpeed; }
	inline void ZeroNumberOfSnakesInGame() { m_numberOfSnakesInGame = Consts::NO_VALUE; }

	// Destruction
	~SharedGame();

private:
	bool m_isInGameSession;
	bool m_isSinglePlayerGame;
	int m_numberOfFramesBeforeGameStart;
	int m_numberOfSnakesInGame;
	int m_playerOneSnakeColorIndex;
	int m_playerTwoSnakeColorIndex;
	int m_randomColumn;
	int m_randomRow;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	int m_snakeCurrentSpeed;
	int m_snakeStartingSpeed;
	int* mp_arrayOfColumnIndices;
	Structure::Vector2 m_randomPosition;
	Structure::Vector2* mp_snakeStartPositions[Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME];
};

#endif SHARED_GAME_H