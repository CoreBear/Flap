#pragma region Includes
#include "SpawnManager.h"

#include "BufferCell.h"
#include "Consts.h";
#include "Enums.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#include "SharedRender.h"
#pragma endregion

#pragma region Initialization
SpawnManager::SpawnManager(ObjectManager& _objectManager, SharedGame& _sharedGame, SharedRender& _sharedRender) :
	mr_objectManager(_objectManager),
	mr_sharedGame(_sharedGame),
	mr_sharedRender(_sharedRender)
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
		
		constexpr int BOOL_CONVERTER = 2;
		if (m_random() % BOOL_CONVERTER)
		{
			SpawnFood();
		}
		else
		{
			SpawnNoTouchy();
		}
	}
}
#pragma endregion

#pragma region Public Functionality
void SpawnManager::Start()
{
	m_spawnTargetFrame = GameManager::s_masterFixedFrameCount + mr_sharedGame.GetNumberOfFramesBeforeGameStart();

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
#pragma endregion

#pragma region Private Functionality
void SpawnManager::GenerateValidRandomPosition()
{
	do
	{
		// Generate random position
		m_randomPosition.m_x = m_random() % (mr_sharedRender.m_bufferHW.X - Consts::OFF_BY_ONE);
		m_randomPosition.m_y = m_random() % (mr_sharedRender.m_bufferHW.Y - Consts::OFF_BY_ONE);
		
		// Get the corresponding buffer cell
		mp_bufferCell = &mr_sharedRender.mp_frameBuffer[(m_randomPosition.m_y * mr_sharedRender.m_bufferHW.X) + m_randomPosition.m_x];

		// If position is invalid (on top of another object). If there's an object already there
	} while (mp_bufferCell->m_objectInCellIndex != Consts::NO_VALUE);
}
void SpawnManager::SpawnFood()
{
	GenerateValidRandomPosition();

	m_genericContainer.m_int = (m_random() % mr_sharedGame.MAX_NUMBER_OF_NODES_TO_ADD) + Consts::OFF_BY_ONE;
	mr_objectManager.SpawnObject(Enums::ObjectType::Food, m_randomPosition, &m_genericContainer);
}
void SpawnManager::SpawnNoTouchy()
{
	GenerateValidRandomPosition();

	mr_objectManager.SpawnObject(Enums::ObjectType::NoTouchy, m_randomPosition);
}
#pragma endregion