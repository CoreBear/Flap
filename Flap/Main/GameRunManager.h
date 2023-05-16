
#ifndef GAME_RUN_MANAGER_H
#define GAME_RUN_MANAGER_H

#include "Consts.h"
#include "Enums.h"

class ObjectManager;
class SharedGame;
class SharedInput;
class SharedNetwork;
class SpawnManager;

class GameRunManager final
{
public:
	// Initialization
	GameRunManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedNetwork& _sharedNetwork);
	GameRunManager(const GameRunManager&) = delete;
	GameRunManager& operator=(const GameRunManager&) = delete;

	// Upates
	void FixedUpdate();
	void LastUpdate();
	void Update();

	// Functionality
	void GameOver();
	void PauseGame();
	void ResumeGame();
	void StartGame(bool _newGame);

	// Destruction
	~GameRunManager();

private:
	// Member Variables
	ObjectManager* const mp_objectManager;
	SharedGame& mr_sharedGame;
	SharedNetwork& mr_sharedNetwork;
	SpawnManager* const mp_spawnManager;
};

#endif GAME_RUN_MANAGER_H