#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameThreadBase.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class CollisionRenderReadOutOfBuffer;
class GameRunManager;
class MenuManager;
class SharedCollisionRender;
class SharedGame;
class SharedInput;

class GameManager final : public GameThreadBase
{
public:
	// Static Variables
	static unsigned int s_fixedFrameCount;

	// Initialization
	GameManager(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame, SharedInput& _sharedInput);
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~GameManager() override;

private:
	// Member Variables
	CollisionRenderReadOutOfBuffer* const mp_collisionRenderReadOutOfBuffer;
	GameRunManager* const mp_gameRunManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	MenuManager* const mp_menuManager;
	SharedGame& mr_sharedGame;
};
#endif GAME_MANAGER_H