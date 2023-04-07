#pragma region Includes
#include "SharedGame.h"

#include "Snake.h"
#pragma endregion

#pragma region Initialization
SharedGame::SharedGame() : m_gameState(Enums::GameState::Menu)
{
	Snake::AssignSharedGame(*this);
}
#pragma endregion