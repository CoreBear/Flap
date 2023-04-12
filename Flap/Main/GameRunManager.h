#ifndef GAME_RUN_MANAGER_H
#define GAME_RUN_MANAGER_H

#include "Consts.h"
#include "Structure.h"

class ObjectManager;
class SharedCollisionRender;
class SharedInput;

class GameRunManager final
{
public:
	// Initialization
	GameRunManager(SharedCollisionRender& _sharedCollisionRender, SharedInput& _sharedInput);
	GameRunManager(const GameRunManager&) = delete;
	GameRunManager& operator=(const GameRunManager&) = delete;

	// Upates
	void FixedUpdate();
	void LastUpdate();
	void Update();

	// Functionality
	void PauseGame();
	void ResumeGame();
	void StartGame();

	// Destruction
	~GameRunManager();

private:
	// Member Variables
	Structure::Generic m_genericContainer;
	int m_reusableIterator;
	ObjectManager* const mp_objectManager;
	Structure::Vector2* mp_snakeStartPositions[Consts::MAX_NUMBER_OF_PLAYERS_PER_GAME];

	// Functionality
	void SetupGame();
};

#endif GAME_RUN_MANAGER_H