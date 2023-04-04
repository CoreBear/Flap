#pragma region Includes
#include "SceneManager.h"

#include "CollisionRenderManager.h"
#include "Consts.h"
#include "ObjectManager.h"
#include "OverlayManager.h"
#include "SharedMemory.h"
#pragma endregion

//HACK:
#include "Structure.h"

#pragma region Initialization
unsigned int SceneManager::s_fixedFrameCount = Consts::NO_VALUE;

SceneManager::SceneManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory) :
	mp_collisionRenderManager(new CollisionRenderManager(_outputWindowHandle, _sharedMemory)),
	mp_objectManager(new ObjectManager(_sharedMemory)),
	mp_overlayManager(new OverlayManager()),
	m_sceneType(SceneType::Overlay),
	mp_sharedMemory(&_sharedMemory)
{
	m_currentTime = m_lastTime = std::chrono::high_resolution_clock::now();

	// HACK:
	{
		Structure::Generic g;
		Structure::Vector2 position;

		// Spawn snake(s)
		{
			position.m_x = 0;
			position.m_y = 0;

			// Snake speed
			g.m_int = 10;

			for (size_t i = 0; i < 2; i++)
			{
				position.m_x = i * 4;

				// Player numbers
				g.m_int2 = i + Consts::OFF_BY_ONE;

				mp_objectManager->SpawnObject(Enums::ObjectType::Avatar, position, &g);
			}
		}

		// Spawn food(s)
		{
			position.m_x = 10;
			position.m_y = 10;

			// How many nodes to add to the snake
			g.m_int = 20;

			mp_objectManager->SpawnObject(Enums::ObjectType::Food, position, &g);
		}
	}
}
#pragma endregion

#pragma region Updates
void SceneManager::Update() 
{
	m_currentTime = std::chrono::high_resolution_clock::now();

	// Fixed Update
	if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
	{
		// Update for next iteration
		m_lastTime = m_currentTime;

		// Update counter
		++s_fixedFrameCount;

		if (m_sceneType == SceneType::Game)
		{
			mp_objectManager->FixedUpdate();

			mp_collisionRenderManager->GameUpdate();
		}
		else
		{
			mp_overlayManager->FixedUpdate();

			mp_collisionRenderManager->OverlayUpdate();
		}
	}

	// Backend/Regular update
	if (m_sceneType == SceneType::Game)
	{
		mp_objectManager->Update();
	}
	else
	{
		mp_overlayManager->Update();
	}

	// Last Update
	if (m_sceneType == SceneType::Game)
	{
		mp_objectManager->LastUpdate();
	}
}
#pragma endregion

#pragma region Destruction
SceneManager::~SceneManager()
{
	delete mp_collisionRenderManager;
	delete mp_objectManager;
	delete mp_overlayManager;
}
#pragma endregion