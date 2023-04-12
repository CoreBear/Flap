#ifndef GAME_RUN_MANAGER_H
#define GAME_RUN_MANAGER_H

#include "Consts.h"
#include "Structure.h"

class CollisionManager;
class ObjectManager;
class SharedGame;
class SharedInput;
class SharedRender;

class GameRunManager final
{
public:
	// Initialization
	GameRunManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender);
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
	void StartGame();

	// Destruction
	~GameRunManager();

private:
	// Member Variables
	CollisionManager* const mp_collisionManager;
	Structure::Generic m_genericContainer;
	int m_reusableIterator;
	ObjectManager* const mp_objectManager;
	SharedGame& mr_sharedGame;
	Structure::Vector2* mp_snakeStartPositions[Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME];

	// Functionality
	void SetupGame();
};

#endif GAME_RUN_MANAGER_H