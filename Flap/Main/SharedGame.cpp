#pragma region Includes
#include "SharedGame.h"

#include "MenuBase.h"
#include "Snake.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame() :
	m_isInGameSession(false),
	m_gameState(Enums::GameState::Menu), 
	m_numberOfSnakesInGame(Consts::NO_VALUE),
	m_snakeStartingSpeed(10)					// NOTE: Arbitrary value
{
	MenuBase::AssignSharedGame(*this);
	Snake::AssignSharedGame(*this);
}
#pragma endregion