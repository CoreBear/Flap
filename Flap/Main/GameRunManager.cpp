#pragma region Includes
#include "GameRunManager.h"

#include "CollisionManager.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"
#pragma endregion

#pragma region Initialization
GameRunManager::GameRunManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender) :
	mp_collisionManager(new CollisionManager(_sharedRender)),
	mp_objectManager(new ObjectManager(_sharedInput, _sharedRender)),
	mr_sharedGame(_sharedGame)
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; m_reusableIterator ++)
	{
		mp_snakeStartPositions[m_reusableIterator] = new Structure::Vector2[static_cast<int>(m_reusableIterator + Consts::OFF_BY_ONE)];
	}

	// HACK: Do this dynamically
	{
		// One player
		{
			mp_snakeStartPositions[0][0].m_x = 20;
			mp_snakeStartPositions[0][0].m_y = 20;
		}

		// Two players
		{
			// Player 1
			mp_snakeStartPositions[1][0].m_x = 10;
			mp_snakeStartPositions[1][0].m_y = 20;

			// Player 2
			mp_snakeStartPositions[1][1].m_x = 30;
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
			mp_snakeStartPositions[3][1].m_x = 30;
			mp_snakeStartPositions[3][1].m_y = 20;

			// Player 3
			mp_snakeStartPositions[3][2].m_x = 10;
			mp_snakeStartPositions[3][2].m_y = 20;

			// Player 4
			mp_snakeStartPositions[3][3].m_x = 30;
			mp_snakeStartPositions[3][3].m_y = 20;
		}
	}
}
#pragma endregion

#pragma region Updates
void GameRunManager::FixedUpdate()
{
	mp_objectManager->FixedUpdate();

	mp_collisionManager->FixedUpdate();
}
void GameRunManager::LastUpdate()
{
	mp_objectManager->LastUpdate();
}
void GameRunManager::Update()
{
	mp_objectManager->Update();
}
#pragma endregion

#pragma region Public Functionality
void GameRunManager::GameOver()
{
	mp_objectManager->CleanScene();

	mr_sharedGame.ZeroNumberOfSnakesInGame();
}
void GameRunManager::PauseGame()
{

}
void GameRunManager::ResumeGame()
{

}
void GameRunManager::StartGame()
{
	SetupGame();
}
#pragma endregion

#pragma region Private Functionality
void GameRunManager::SetupGame()
{
	// HACk: Snake speed
	m_genericContainer.m_int = 10;

	// HACK: Figure out who is an avatar and who is a snake (piloted by networked player or AI)
	
	// HACK: Make this all dynamic
	mr_sharedGame.IncrementNumberOfSnakesInGame();
	int numberOfPlayers = mr_sharedGame.GetNumberOfSnakesInGame();
	int numberOfPlayerIndex = numberOfPlayers - Consts::OFF_BY_ONE;

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < numberOfPlayers; m_reusableIterator++)
	{
		// Snake number
		m_genericContainer.m_int2 = m_reusableIterator + Consts::OFF_BY_ONE;

		if (m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM)
		{
			mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, mp_snakeStartPositions[numberOfPlayerIndex][m_reusableIterator], &m_genericContainer);
		}
		else
		{
			mp_objectManager->SpawnObject(Enums::ObjectType::Snake, mp_snakeStartPositions[numberOfPlayerIndex][m_reusableIterator], &m_genericContainer);
		}
	}

	// HACK: Just to add some food
	Structure::Vector2 position;
	position.m_x = 10;
	position.m_y = 10;
	m_genericContainer.m_int = 20;
	mp_objectManager->SpawnObject(Enums::ObjectType::Food, position, &m_genericContainer);
}
#pragma endregion

#pragma region Destruction
GameRunManager::~GameRunManager()
{
	delete mp_collisionManager;
	delete mp_objectManager;

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; m_reusableIterator++)
	{
		delete mp_snakeStartPositions[m_reusableIterator];
	}

}
#pragma endregion