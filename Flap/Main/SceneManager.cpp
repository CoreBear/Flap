#pragma region Includes
#include "SceneManager.h"

#include "CollisionRenderBufferToObjectAndScreen.h"
#include "Consts.h"
#include "MenuManager.h"
#include "ObjectManager.h"
#include "SharedCollisionRender.h"
#include "SharedInput.h"
#pragma endregion

//HACK:
#include "Structure.h"

#pragma region Initialization
unsigned int SceneManager::s_fixedFrameCount = Consts::NO_VALUE;

SceneManager::SceneManager(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender, SharedInput& _sharedInput) :
	mp_collisionRenderBufferToObjectAndScreen(new CollisionRenderBufferToObjectAndScreen(_outputWindowHandle, _sharedCollisionRender)),
	mp_menuManager(new MenuManager(_sharedCollisionRender)),
	mp_objectManager(new ObjectManager(_sharedCollisionRender, _sharedInput)),
	mr_sharedCollisionRender(_sharedCollisionRender),
	mr_sharedInput(_sharedInput)
{
	_sharedInput.m_inputSceneType = Enums::InputSceneType::Menu;

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
	// NOTE/WARNING: Keep the if/elses

	mr_sharedInput.m_inputSceneTypeMutex.lock();

	if (mr_sharedInput.m_inputSceneType == Enums::InputSceneType::Game)
	{
		mr_sharedInput.m_inputSceneTypeMutex.unlock();

		m_currentTime = std::chrono::high_resolution_clock::now();

		// Fixed Update
		if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
		{
			// Update for next iteration
			m_lastTime = m_currentTime;

			// Update counter
			++s_fixedFrameCount;

			mp_objectManager->FixedUpdate();

			mp_collisionRenderBufferToObjectAndScreen->FixedUpdate();
		}

		mp_objectManager->Update();

		mp_objectManager->LastUpdate();
	}
	else
	{
		mr_sharedInput.m_inputSceneTypeMutex.unlock();

		m_currentTime = std::chrono::high_resolution_clock::now();

		// Fixed Update
		if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
		{
			// Update for next iteration
			m_lastTime = m_currentTime;

			// Update counter
			++s_fixedFrameCount;

			mp_menuManager->FixedUpdate();

			mp_collisionRenderBufferToObjectAndScreen->FixedUpdate();
		}

		mp_menuManager->Update();
	}
}
#pragma endregion

#pragma region Destruction
SceneManager::~SceneManager()
{
	delete mp_collisionRenderBufferToObjectAndScreen;
	delete mp_objectManager;
	delete mp_menuManager;
}
#pragma endregion