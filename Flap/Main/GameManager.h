#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameThreadBase.h"

#include "Enums.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class CollisionManager;
class FileIOManager;
class GameRunManager;
class MenuManager;
class NetworkInputUpdater;
class NetworkManager;
class SharedGame;
class SharedInput;
class SharedNetwork;
class SharedGame;

class GameManager final : public GameThreadBase
{
public:
	// Static Variables
	static unsigned int s_masterFixedFrameCount;

	// Initialization
	GameManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame, SharedInput& _sharedInput);
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~GameManager() override;

private:
	// Member Variables
	bool m_newHighScore;
	CollisionManager* const mp_collisionManager;
	FileIOManager* const mp_fileIOManager;
	SharedNetwork* const mp_sharedNetwork;	// Out of order because certain managers need it
	GameRunManager* const mp_gameRunManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	MenuManager* const mp_menuManager;
	NetworkInputUpdater* mp_networkInputUpdater;
	NetworkManager* mp_networkManager;		// NOTE: Allocated and destroyed at specific times so winsock isn't always initialized
	SharedGame& mr_sharedGame;
	SharedInput& mr_sharedInput;

	// Functionality
	void GameOver();
	void SetGameActivity(int _gameActivityIndex);
	void SetInputType(Enums::InputType _inputType);
};
#endif GAME_MANAGER_H