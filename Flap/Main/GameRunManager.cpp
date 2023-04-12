#pragma region Includes
#include "GameRunManager.h"

#include "ObjectManager.h"
#include "SharedCollisionRender.h"
#include "SharedInput.h"
#pragma endregion

#pragma region Initialization
GameRunManager::GameRunManager(SharedCollisionRender& _sharedCollisionRender, SharedInput& _sharedInput) :
	mp_objectManager(new ObjectManager(_sharedCollisionRender, _sharedInput))
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
	// Snake speed
	m_genericContainer.m_int = 10;

	// HACK: Figure out who is an avatar and who is a snake (piloted by networked player or AI)
	
	// HACK: Make this all dynamic
	int numberOfPlayers = 1;

	switch (numberOfPlayers)
	{
	case 1:
	{
		// Player number
		m_genericContainer.m_int2 = numberOfPlayers;

		mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, mp_snakeStartPositions[0][0], &m_genericContainer);
	}
	break;
	case 2:
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < numberOfPlayers; m_reusableIterator++)
		{
			// Player number
			m_genericContainer.m_int2 = m_reusableIterator + Consts::OFF_BY_ONE;

			if (m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM)
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, mp_snakeStartPositions[1][m_reusableIterator], &m_genericContainer);
			}
			else
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Snake, mp_snakeStartPositions[1][m_reusableIterator], &m_genericContainer);
			}
		}
	}
		break;
	case 3:
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < numberOfPlayers; m_reusableIterator++)
		{
			// Player number
			m_genericContainer.m_int2 = m_reusableIterator + Consts::OFF_BY_ONE;

			if (m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM)
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, mp_snakeStartPositions[2][m_reusableIterator], &m_genericContainer);
			}
			else
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Snake, mp_snakeStartPositions[2][m_reusableIterator], &m_genericContainer);
			}
		}
	}
		break;
	case 4:
	{
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < numberOfPlayers; m_reusableIterator++)
		{
			// Player number
			m_genericContainer.m_int2 = m_reusableIterator + Consts::OFF_BY_ONE;

			if (m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM)
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, mp_snakeStartPositions[3][m_reusableIterator], &m_genericContainer);
			}
			else
			{
				mp_objectManager->SpawnObject(Enums::ObjectType::Snake, mp_snakeStartPositions[3][m_reusableIterator], &m_genericContainer);
			}
		}
	}
		break;
	}
}
#pragma endregion

#pragma region Destruction
GameRunManager::~GameRunManager()
{
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME; m_reusableIterator++)
	{
		delete[] mp_snakeStartPositions[m_reusableIterator];
	}

	delete mp_objectManager;
}
#pragma endregion