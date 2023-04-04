#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Manager.h"

#include <chrono>
#include <Windows.h>		// NOTE/WARNING: Has to be here or else the compiler screams about forward declaring HANDLE

class CollisionRenderManager;
class ObjectManager;
class OverlayManager;
class SharedMemory;

class SceneManager final : public Manager
{
public:
	// Static Variables
	static unsigned int s_fixedFrameCount;

	// Initialization
	SceneManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory);
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~SceneManager() override;

private:
	// Static Variables
	enum class SceneType { Game, Overlay };

	// Member Variables
	CollisionRenderManager* const mp_collisionRenderManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	ObjectManager* const mp_objectManager;
	OverlayManager* const mp_overlayManager;
	SceneType m_sceneType;
	SharedMemory* const mp_sharedMemory;
};
#endif SCENE_MANAGER_H