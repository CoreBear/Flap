#pragma region Includes
#include "SceneManager.h"

#include "AtomicMemory.h"
#include "Consts.h"
#include "ObjectManager.h"
#include "OverlayManager.h"
#pragma endregion

//HACK:
#include "Structure.h"

#pragma region Initialization
unsigned int SceneManager::s_simFrameCount = Consts::NO_VALUE;

SceneManager::SceneManager(AtomicMemory& _atomicMemory) : mp_atomicMemory(&_atomicMemory), m_sceneType(SceneType::Game)
{
	m_currentTime = m_lastTime = std::chrono::high_resolution_clock::now();

	mp_objectManager = new ObjectManager(_atomicMemory);
	mp_overlayManager = new OverlayManager();

	// HACK:
	{
		Structure::Vector2<int> position;
		position.m_x = 0;
		position.m_y = 0;

		Structure::Generic g;

		mp_objectManager->SpawnObject(Enums::ObjectType::Bird, position, g);
	}
}
#pragma endregion

#pragma region Updates
void SceneManager::Update() 
{
	// Backend/Regular update
	if (m_sceneType == SceneType::Game)
	{
		mp_objectManager->Update();
	}
	else
	{
		mp_overlayManager->Update();
	}

	// Fixed Update
	m_currentTime = std::chrono::high_resolution_clock::now();

	if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
	{
		// Update for next iteration
		m_lastTime = m_currentTime;

		// Update counter
		++s_simFrameCount;

		if (m_sceneType == SceneType::Game)
		{
			mp_objectManager->FixedUpdate();
		}
		else
		{
			mp_overlayManager->FixedUpdate();
		}

		mp_atomicMemory->FixedUpdateCycleCompleted();
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
	delete mp_objectManager;
	delete mp_overlayManager;
}
#pragma endregion