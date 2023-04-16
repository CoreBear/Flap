#pragma region Includes
#include "GameManager.h"

#include "Consts.h"
#include "Enums.h"
#include "FileIOManager.h"
#include "GameRunManager.h"
#include "MenuManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"
#pragma endregion

#pragma region Initialization
unsigned int GameManager::s_masterFixedFrameCount = Consts::NO_VALUE;

GameManager::GameManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender) :
	mp_fileIOManager(new FileIOManager(_sharedGame)),
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
			++s_masterFixedFrameCount;

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

		mp_menuManager->ToMain();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ExitToResults:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		if (mr_sharedGame.GetSinglePlayerBool())
		{
			m_newHighScore = mp_fileIOManager->CheckForNewHighScore();

			mp_menuManager->ToResultsSingle(m_newHighScore);
		}
		else
		{
			mp_menuManager->ToResultsMulti();
		}

		GameOver();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::LoadGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->LoadGame();

		mr_sharedGame.GameSession(true, true);

		mp_gameRunManager->StartGame(false);

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
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
			++s_masterFixedFrameCount;

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
	case Enums::GameState::SaveGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->SaveGame();

		GameOver();

		mp_menuManager->ToMain();
	}
	break;
	case Enums::GameState::SaveHighScores:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->SaveHighScores(m_newHighScore, "BBB");

		mp_menuManager->ToMain();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::StartGameLocal:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mr_sharedGame.GameSession(true, false);

		mp_gameRunManager->StartGame(true);

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Game;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::StartGameSingle:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mr_sharedGame.GameSession(true, true);

		mp_gameRunManager->StartGame(true);

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
	mr_sharedGame.GameSession(false, false);

	mr_sharedRender.ResetFrameBufferSynced();

	mp_gameRunManager->GameOver();
}
#pragma endregion

#pragma region Destruction
GameManager::~GameManager()
{
	delete mp_fileIOManager;
	delete mp_gameRunManager;
	delete mp_menuManager;
}
#pragma endregion