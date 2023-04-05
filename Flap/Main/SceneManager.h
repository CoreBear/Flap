#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "GameThreadBase.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class CollisionRenderBufferToObjectAndScreen;
class ObjectManager;
class MenuManager;
class SharedCollisionRender;
class SharedInput;

class SceneManager final : public GameThreadBase
{
public:
	// Static Variables
	static unsigned int s_fixedFrameCount;

	// Initialization
	SceneManager(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender, SharedInput& _sharedInput);
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~SceneManager() override;

private:
	// Member Variables
	CollisionRenderBufferToObjectAndScreen* const mp_collisionRenderBufferToObjectAndScreen;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	MenuManager* const mp_menuManager;
	ObjectManager* const mp_objectManager;
	SharedCollisionRender& mr_sharedCollisionRender;
	SharedInput& mr_sharedInput;
};
#endif SCENE_MANAGER_H