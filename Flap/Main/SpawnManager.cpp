#pragma region Includes
#include "SpawnManager.h"

#include "BufferCell.h"
#include "Consts.h"
#include "Enums.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#pragma endregion

#pragma region Initialization
SpawnManager::SpawnManager(ObjectManager& _objectManager, SharedGame& _sharedGame) :
	mr_objectManager(_objectManager),
	mr_sharedGame(_sharedGame)
{
	return;
}
#pragma endregion

#pragma region MyRegion
void SpawnManager::FixedUpdate()
{
	if (GameManager::s_masterFixedFrameCount == m_spawnTargetFrame)
	{
		m_spawnTargetFrame = GameManager::s_masterFixedFrameCount + mr_sharedGame.m_numberOfFramesBetweenSpawn;
		
		// NOTE: The higher this number, the higher chance no touchy's spawn
		constexpr int BOOL_CONVERTER = 3;
		if (mr_sharedGame.m_random() % BOOL_CONVERTER)
		{
			SpawnNoTouchy();
		}
		else
		{
			SpawnFood();
		}
	}
}
#pragma endregion

#pragma region Public Functionality
void SpawnManager::Start(bool _newGame)
{
	m_spawnTargetFrame = GameManager::s_masterFixedFrameCount + mr_sharedGame.GetNumberOfFramesBeforeGameStart();

	if (_newGame)
	{
		// HACK: Figure out who is an avatar and who is a snake (piloted by networked player or AI)

		// Spawn avatars and snakes
		{
			m_numberOfPlayersIndex = mr_sharedGame.GetNumberOfSnakesInGame() - Consts::OFF_BY_ONE;

			for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedGame.GetNumberOfSnakesInGame(); m_reusableIterator++)
			{
				// Snake number
				m_genericContainer.m_int = m_reusableIterator + Consts::OFF_BY_ONE;

				if (m_reusableIterator < Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM)
				{
					mr_objectManager.SpawnObject(Enums::ObjectType::Avatar, mr_sharedGame.GetSnakeStartPositionRef(m_numberOfPlayersIndex, m_reusableIterator), &m_genericContainer);
				}
				else
				{
					mr_objectManager.SpawnObject(Enums::ObjectType::Snake, mr_sharedGame.GetSnakeStartPositionRef(m_numberOfPlayersIndex, m_reusableIterator), &m_genericContainer);
				}
			}
		}
	}
}
#pragma endregion

#pragma region Private Functionality
void SpawnManager::SpawnFood()
{
	m_randomPosition = mr_sharedGame.GetRandomSpawnPositionRef();

	m_genericContainer.m_int = (mr_sharedGame.m_random() % mr_sharedGame.MAX_NUMBER_OF_NODES_TO_ADD) + Consts::OFF_BY_ONE;
	mr_objectManager.SpawnObject(Enums::ObjectType::Food, m_randomPosition, &m_genericContainer);
}
void SpawnManager::SpawnNoTouchy()
{
	m_randomPosition = mr_sharedGame.GetRandomSpawnPositionRef();

	mr_objectManager.SpawnObject(Enums::ObjectType::NoTouchy, m_randomPosition);
}
#pragma endregion