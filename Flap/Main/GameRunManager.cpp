#pragma region Includes
#include "GameRunManager.h"

#include "CollisionManager.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#include "SharedInput.h"
#include "SharedRender.h"
#include "SpawnManager.h"
#pragma endregion

#pragma region Initialization
GameRunManager::GameRunManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender) :
	mp_collisionManager(new CollisionManager(_sharedRender)),
	mp_objectManager(new ObjectManager(_sharedGame, _sharedInput, _sharedRender)),
	mr_sharedGame(_sharedGame),
	mp_spawnManager(new SpawnManager(*mp_objectManager, _sharedGame))
{
	return;
}
#pragma endregion

#pragma region Updates
void GameRunManager::FixedUpdate()
{
	mp_objectManager->FixedUpdate();

	mp_collisionManager->FixedUpdate();

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
	switch (mr_sharedGame.m_gameState)
	{
	case Enums::GameState::StartGameLocal:
		mr_sharedGame.IncrementNumberOfSnakesInGame();

		// NOTE: Notice the fallthrough
	case Enums::GameState::StartGameSingle:
		mr_sharedGame.IncrementNumberOfSnakesInGame();
		break;
	}

	mp_objectManager->Start(_newGame);

	mr_sharedGame.ResetAvailableSpawnIndices();

	mp_spawnManager->Start(_newGame);
}
#pragma endregion

#pragma region Destruction
GameRunManager::~GameRunManager()
{
	delete mp_collisionManager;
	delete mp_objectManager;
	delete mp_spawnManager;
}
#pragma endregion