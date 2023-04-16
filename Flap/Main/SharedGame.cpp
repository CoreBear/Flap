#pragma region Includes
#include "SharedGame.h"

#include "MenuBase.h"
#include "Snake.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame() :
	m_isInGameSession(false),
	m_gameState(Enums::GameState::Menu),
	MAX_NUMBER_OF_NODES_TO_ADD(9),				// NOTE: Arbitrary value
	m_numberOfSnakesInGame(Consts::NO_VALUE),
	m_numberOfFramesBeforeGameStart(60),		// NOTE: Arbitrary value
	m_numberOfFramesBetweenSpawn(60),			// NOTE: Arbitrary value
	m_snakeStartingSpeed(10)					// NOTE: Arbitrary value
{
	MenuBase::AssignSharedGame(*this);
	Snake::AssignSharedGame(*this);

	for (int positionIndex = Consts::NO_VALUE; positionIndex < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; positionIndex++)
	{
		mp_snakeStartPositions[positionIndex] = new Structure::Vector2[static_cast<int>(positionIndex + Consts::OFF_BY_ONE)];
	}

	// HACK: Do this dynamically and use calculated positions
	{
		// One player
		{
			mp_snakeStartPositions[0][0].m_x = 10;
			mp_snakeStartPositions[0][0].m_y = 20;
		}

		// Two players
		{
			// Player 1
			mp_snakeStartPositions[1][0].m_x = 10;
			mp_snakeStartPositions[1][0].m_y = 20;

			// Player 2
			mp_snakeStartPositions[1][1].m_x = 20;
			mp_snakeStartPositions[1][1].m_y = 20;
		}

		// Three players
		{
			// Player 1
			mp_snakeStartPositions[2][0].m_x = 10;
			mp_snakeStartPositions[2][0].m_y = 20;

			// Player 2			   
			mp_snakeStartPositions[2][1].m_x = 20;
			mp_snakeStartPositions[2][1].m_y = 20;

			// Player 3			   
			mp_snakeStartPositions[2][2].m_x = 30;
			mp_snakeStartPositions[2][2].m_y = 20;
		}

		// Four players
		{
			// Player 1
			mp_snakeStartPositions[3][0].m_x = 10;
			mp_snakeStartPositions[3][0].m_y = 20;

			// Player 2
			mp_snakeStartPositions[3][1].m_x = 20;
			mp_snakeStartPositions[3][1].m_y = 20;

			// Player 3
			mp_snakeStartPositions[3][2].m_x = 30;
			mp_snakeStartPositions[3][2].m_y = 20;

			// Player 4
			mp_snakeStartPositions[3][3].m_x = 40;
			mp_snakeStartPositions[3][3].m_y = 20;
		}
	}
}
#pragma endregion

#pragma region Public Functionality
void SharedGame::GameSession(bool _isInGameSession, bool _isSinglePlayerGame)
{
	m_isInGameSession = _isInGameSession;
	m_isSinglePlayerGame = _isSinglePlayerGame;
}
#pragma endregion

#pragma region Destruction
SharedGame::~SharedGame()
{
	for (int positionIndex = Consts::NO_VALUE; positionIndex < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; positionIndex++)
	{
		delete mp_snakeStartPositions[positionIndex];
	}
}
#pragma endregion