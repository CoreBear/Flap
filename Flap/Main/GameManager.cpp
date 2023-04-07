#pragma region Includes
#include "GameManager.h"

#include "CollisionRenderReadOutOfBuffer.h"
#include "Consts.h"
#include "Enums.h"
#include "GameRunManager.h"
#include "MenuManager.h"
#include "SharedCollisionRender.h"
#include "SharedGame.h"
#include "SharedInput.h"
#pragma endregion

#pragma region Initialization
unsigned int GameManager::s_fixedFrameCount = Consts::NO_VALUE;

GameManager::GameManager(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame, SharedInput& _sharedInput) :
	mp_collisionRenderReadOutOfBuffer(new CollisionRenderReadOutOfBuffer(_outputWindowHandle, _sharedCollisionRender)),
	mp_gameRunManager(new GameRunManager(_sharedCollisionRender, _sharedGame, _sharedInput)),
	mp_menuManager(new MenuManager(_sharedCollisionRender, _sharedGame)),
	mr_sharedCollisionRender(_sharedCollisionRender),
	mr_sharedGame(_sharedGame)
{
	m_currentTime = m_lastTime = std::chrono::high_resolution_clock::now();
}
#pragma endregion

#pragma region Updates
void GameManager::Update()
{
	mr_sharedGame.m_gameStateMutex.lock();

	switch (mr_sharedGame.m_gameState)
	{
	case Enums::GameState::Game:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		m_currentTime = std::chrono::high_resolution_clock::now();

		// Fixed Update
		if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
		{
			// Update for next iteration
			m_lastTime = m_currentTime;

			// Update counter
			++s_fixedFrameCount;

			mp_gameRunManager->FixedUpdate();

			mp_collisionRenderReadOutOfBuffer->FixedUpdate();
		}

		mp_gameRunManager->Update();

		mp_gameRunManager->LastUpdate();
	}
	break;
	case Enums::GameState::ExitToMain:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->CleanScene();
		mp_menuManager->ExitToMain();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedCollisionRender.m_menuConVar.notify_one();
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ExitToResults:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->CleanScene();
		mp_menuManager->ExitToResults();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::Menu:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		m_currentTime = std::chrono::high_resolution_clock::now();

		// Fixed Update
		if (std::chrono::duration_cast<std::chrono::microseconds>(m_currentTime - m_lastTime).count() >= Consts::FIXED_DELTA_TIME_LL)
		{
			// Update for next iteration
			m_lastTime = m_currentTime;

			// Update counter
			++s_fixedFrameCount;

			mp_menuManager->FixedUpdate();


			if (mr_sharedGame.m_gameState == Enums::GameState::ExitApp)
			{
				// Run functionality exit here...

				mr_sharedCollisionRender.m_menuConVar.notify_one();
				return;
			}

			mp_collisionRenderReadOutOfBuffer->FixedUpdate();
		}

		mp_menuManager->Update();
	}
	break;
	case Enums::GameState::PauseGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->PauseGame();
		mp_menuManager->PauseGame();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ResumeGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->ResumeGame();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
		mr_sharedCollisionRender.m_menuConVar.notify_one();
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::StartGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->StartGame();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
		//mr_sharedCollisionRender.m_menuConVar.notify_one();
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	default:
		mr_sharedGame.m_gameStateMutex.unlock();
		break;
	}
}
#pragma endregion

#pragma region Destruction
GameManager::~GameManager()
{
	delete mp_collisionRenderReadOutOfBuffer;
	delete mp_gameRunManager;
	delete mp_menuManager;
}
#pragma endregion