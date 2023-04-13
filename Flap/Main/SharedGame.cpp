#pragma region Includes
#include "SharedGame.h"

#include "MenuBase.h"
#include "Snake.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame() :
	m_isInGameSession(false),
	m_gameState(Enums::GameState::Menu), 
	m_numberOfSnakesInGame(Consts::NO_VALUE) 
{
	MenuBase::AssignSharedGame(*this);
	Snake::AssignSharedGame(*this);
}
#pragma endregion