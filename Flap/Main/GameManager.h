#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameThreadBase.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class FileIOManager;
class GameRunManager;
class MenuManager;
class NetworkManager;
class SharedGame;
class SharedInput;
class SharedNetwork;
class SharedRender;

class GameManager final : public GameThreadBase
{
public:
	// Static Variables
	static unsigned int s_masterFixedFrameCount;

	// Initialization
	GameManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender);
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~GameManager() override;

private:
	// Member Variables
	bool m_newHighScore;
	FileIOManager* const mp_fileIOManager;
	GameRunManager* const mp_gameRunManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	SharedNetwork* const mp_sharedNetwork;
	MenuManager* const mp_menuManager;
	NetworkManager* mp_networkManager;		// NOTE: Allocated and destroyed at specific times so winsock isn't always initialized
	SharedGame& mr_sharedGame;
	SharedInput& mr_sharedInput;
	SharedRender& mr_sharedRender;

	// Functionality
	void GameOver();
};
#endif GAME_MANAGER_H