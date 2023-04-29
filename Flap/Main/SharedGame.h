#ifndef SHARED_GAME_H
#define SHARED_GAME_H

#include "Consts.h"
#include "Enums.h"
#include "Structure.h"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <random>

class BufferCell;
template<class Type> class Queue;

class SharedGame final
{
public:
	// Static Variables
	static constexpr int MAX_NUMBER_OF_INITIALS = 3;
	static constexpr int MAX_HS_STRING_LENGTH = 50;			// NOTE: Arbitrary value

	// Member Variables
	bool m_borderIsRequired;
	BufferCell** mpp_frameBuffer;
	char m_initials[MAX_NUMBER_OF_INITIALS]{ ' ',' ',' ' };
	char** mpp_highScoreLines;
	std::condition_variable m_frameBufferConVar;
	int m_gameActivityIndex;
	int m_largestSnakeLengthUponDeath;
	int m_numberOfFramesBetweenSpawn;
	const int MAX_NUMBER_OF_NODES_TO_ADD;
	static constexpr int MAX_NUMBER_OF_HIGH_SCORES = 10;	// NOTE: Arbitrary value
	std::mutex m_frameBufferMutex;
	std::mutex m_gameActivityIndexMutex;
	Queue<int>* mp_availableSpawnPositions;
	std::random_device m_random;
	unsigned short m_largestSnakeColor;
	const Structure::Vector2<short> FRAME_BUFFER_HEIGHT_WIDTH;
	Structure::Vector4<short> m_gameAreaBounds;

	// Initialization
	SharedGame(const Structure::Vector2<short>& _frameBufferWidthHeight);
	SharedGame(const SharedGame&) = delete;
	SharedGame& operator=(const SharedGame&) = delete;

	// Functionality
	void AddAvailableSpawnIndex(int _x, int _y);
	inline void DecrementNumberOfSnakesInGame() { --m_numberOfSnakesInGame; }
	void EndGameSession();
	inline bool GetIsInGameSession() const { return m_isInGameSession; }
	inline int GetNumberOfFramesBeforeGameStart() const { return m_numberOfFramesBeforeGameStart; }
	inline int GetNumberOfSnakesInGame() const { return m_numberOfSnakesInGame; }
	inline int& GetPlayerSnakeColorIndexRef(int _playerIndex) { return (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex : m_playerTwoSnakeColorIndex; }
	const Structure::Vector2<int>& GetRandomSpawnPositionRef();
	inline bool GetSinglePlayerBool() const { return m_isSinglePlayerGame; }
	const Structure::Vector2<int>& GetSnakeStartPositionRef(int _numberOfPlayersIndex, int _playerIndex);
	inline void IncrementNumberOfSnakesInGame() { ++m_numberOfSnakesInGame; }
	void RemoveAvailableSpawnIndex(int _x, int _y);
	void ResetAvailableSpawnIndices();
	inline void ResetLargestClientOffsets() { m_clientSharedGameAreaOffsets.m_x = m_clientSharedGameAreaOffsets.m_y = SHRT_MAX; }
	void ResetFrameBuffer();
	void ResetFrameBufferSynced();
	inline void SetSinglePlayerBool(bool _isSinglePlayerGame) { m_isSinglePlayerGame = _isSinglePlayerGame; }
	inline void SetPlayerSnakeColorIndex(int _colorIndex, int _playerIndex) { (_playerIndex == Consts::NO_VALUE) ? m_playerOneSnakeColorIndex = _colorIndex : m_playerTwoSnakeColorIndex = _colorIndex; }
	void StartGameSession(bool _isInGameSession, bool _isSinglePlayerGame);
	void TryToggleBorder(bool _isVisible);
	void UpdateClientSharedGameAreaOffsets(const Structure::Vector2<short>& _clientOffsets);
	void UpdateMyGameAreaBounds(short _xOffset, short _yOffset);
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
	int m_reusableIterator_3;
	int m_reusableIterator_4;
	int* mp_arrayOfColumnIndices;
	Structure::Vector2<int> m_randomPosition;
	Structure::Vector2<int>* mp_snakeStartPositions[Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME];
	Structure::Vector2<short> m_clientSharedGameAreaOffsets;
};

#endif SHARED_GAME_H