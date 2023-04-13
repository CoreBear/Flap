#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameThreadBase.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class GameRunManager;
class MenuManager;
class SharedGame;
class SharedInput;
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
	GameRunManager* const mp_gameRunManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	MenuManager* const mp_menuManager;
	SharedGame& mr_sharedGame;
	SharedRender& mr_sharedRender;

	// Functionality
	void GameOver();
};
#endif GAME_MANAGER_H