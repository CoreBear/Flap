#pragma region Includes
#include "GameManager.h"

#include "Consts.h"
#include "Enums.h"
#include "FileIOManager.h"
#include "GameRunManager.h"
#include "MenuManager.h"
#include "NetworkManager.h"
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
	mr_sharedInput(_sharedInput),
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

		mp_menuManager->DisplayMenu(Enums::MenuName::Main);

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

			if (m_newHighScore)
			{
				mr_sharedInput.m_inputTypeMutex.lock();
				mr_sharedInput.m_inputType = Enums::InputType::HighScore;
				mr_sharedInput.m_inputTypeMutex.unlock();

				mp_menuManager->DisplayMenu(Enums::MenuName::NewHighScore);
			}
			else
			{
				mp_menuManager->DisplayMenu(Enums::MenuName::HighScore);
			}
		}
		else
		{
			mp_menuManager->DisplayMenu(Enums::MenuName::ResultsMulti);
		}

		GameOver();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::HighScoreToMain:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->ClearHighScores();

		if (mp_menuManager->PreviousMenuIs(Enums::MenuName::Main))
		{
			mp_menuManager->ReturnToPreviousMenu();
		}
		else
		{
			mp_menuManager->DisplayMenu(Enums::MenuName::Main);
		}

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
		mp_menuManager->DisplayMenu(Enums::MenuName::Pause);

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
	case Enums::GameState::RunAsClient:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_networkManager->InitHost(false);

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::RunAsServer:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_networkManager->InitHost(true);

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::SaveGame:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->SaveGame();

		GameOver();

		mp_menuManager->DisplayMenu(Enums::MenuName::Main);
	}
	break;
	case Enums::GameState::SaveHighScores:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mr_sharedInput.m_inputTypeMutex.lock();
		mr_sharedInput.m_inputType = Enums::InputType::Normal;
		mr_sharedInput.m_inputTypeMutex.unlock();

		mp_fileIOManager->SaveHighScores(m_newHighScore, mr_sharedGame.m_initials);

		mp_menuManager->DisplayMenu(Enums::MenuName::HighScore);

		mp_fileIOManager->ClearHighScores();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::ShowHighScores:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_fileIOManager->LoadHighScores();

		mp_menuManager->DisplayMenu(Enums::MenuName::HighScore);

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
	case Enums::GameState::StartNetworking:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_networkManager = new NetworkManager(mr_sharedGame);
		m_networkingThread = std::thread(&NetworkManager::NetworkThreadEntry_Loop, &(*mp_networkManager));

		mp_menuManager->DisplayMenu(Enums::MenuName::Network);

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
		mr_sharedGame.m_gameStateMutex.unlock();
	}
	break;
	case Enums::GameState::StopNetworking:
	{
		mr_sharedGame.m_gameStateMutex.unlock();

		mp_networkManager->StopNetworking();
		m_networkingThread.join();
		delete mp_networkManager;

		mp_menuManager->ReturnToPreviousMenu();

		mr_sharedGame.m_gameStateMutex.lock();
		mr_sharedGame.m_gameState = Enums::GameState::Menu;
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