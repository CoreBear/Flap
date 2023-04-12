#pragma region Includes
#include "GameManager.h"

#include "Consts.h"
#include "Enums.h"
#include "GameRunManager.h"
#include "MenuManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"
#pragma endregion

#pragma region Initialization
unsigned int GameManager::s_fixedFrameCount = Consts::NO_VALUE;

GameManager::GameManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender) :
	mp_gameRunManager(new GameRunManager(_sharedGame, _sharedInput, _sharedRender)),
	mp_menuManager(new MenuManager(_sharedGame, _sharedRender)),
	mr_sharedGame(_sharedGame),
	mr_sharedRender(_sharedRender)
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

			//mp_collisionRenderReadOutOfBuffer->FixedUpdate();
		}

		mp_gameRunManager->Update();

		mp_gameRunManager->LastUpdate();
	}
	break;
	case Enums::GameState::ExitToMain:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		GameOver();

		mp_menuManager->ExitToMain();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ExitToResults:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		GameOver();

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

			//mp_collisionRenderReadOutOfBuffer->FixedUpdate();
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
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ResumeGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->ResumeGame();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::StartGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_gameRunManager->StartGame();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	default:
		mr_sharedGame.m_gameStateMutex.unlock();
		break;
	}
}
#pragma endregion

#pragma region Private Functionality
void GameManager::GameOver()
{
	mr_sharedRender.ResetFrameBufferSynced();

	mp_gameRunManager->GameOver();
}
#pragma endregion

#pragma region Destruction
GameManager::~GameManager()
{
	delete mp_gameRunManager;
	delete mp_menuManager;
}
#pragma endregion