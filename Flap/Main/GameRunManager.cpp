#pragma region Includes
#include "GameRunManager.h"

#include "ObjectManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedNetwork.h"
#include "SpawnManager.h"
#pragma endregion

#pragma region Initialization
GameRunManager::GameRunManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedNetwork& _sharedNetwork) :
	mp_objectManager(new ObjectManager(_sharedGame, _sharedInput)),
	mr_sharedGame(_sharedGame),
	mr_sharedNetwork(_sharedNetwork),
	mp_spawnManager(new SpawnManager(*mp_objectManager, _sharedGame))
{
	return;
}
#pragma endregion

#pragma region Updates
void GameRunManager::FixedUpdate()
{
	mp_objectManager->FixedUpdate();

	mp_spawnManager->FixedUpdate();
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
void GameRunManager::GameOver()
{
	mp_objectManager->CleanScene();

	mr_sharedGame.ZeroNumberOfSnakesInGame();
}
void GameRunManager::PauseGame()
{
	mp_objectManager->Pause();
}
void GameRunManager::ResumeGame()
{
	mp_objectManager->Resume();
}
void GameRunManager::StartGame(bool _newGame)
{
	mr_sharedGame.ResetAvailableSpawnIndices();

	switch (mr_sharedGame.m_gameActivityIndex)
	{
	// NOTE: Only server will come in here
	case Enums::GameActivity::PlayGameMulti:
	{
		// Add 1, because this server is also a player
		mr_sharedGame.AssignNumberOfSnakesInGame(mr_sharedNetwork.m_numOfConnClientsOnServInt + Consts::OFF_BY_ONE);

		mp_spawnManager->Start(false);
	}
	break;

	case Enums::GameActivity::PlayGameLocal:
		mr_sharedGame.IncrementNumberOfSnakesInGame();

		// NOTE: Notice the fallthrough!
	case Enums::GameActivity::PlayGameSingle:
	{
		mr_sharedGame.IncrementNumberOfSnakesInGame();

		if (_newGame)
		{
			mp_spawnManager->Start(true);
		}

		// Loading game
		else
		{
			mp_objectManager->Start(true);
		}
	}
	break;
	}
}
#pragma endregion

#pragma region Destruction
GameRunManager::~GameRunManager()
{
	delete mp_objectManager;
	delete mp_spawnManager;
}
#pragma endregion