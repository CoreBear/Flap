#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Manager.h"

#include <chrono>

class CollisionManager;
class ObjectManager;
class OverlayManager;
class SharedMemory;

class SceneManager final : public Manager
{
public:
	// Static Variables
	static unsigned int s_fixedFrameCount;

	// Initialization
	SceneManager(SharedMemory& _sharedMemory);
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
	CollisionManager* const mp_collisionManager;
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	ObjectManager* const mp_objectManager;
	OverlayManager* const mp_overlayManager;
	SceneType m_sceneType;
	SharedMemory* const mp_sharedMemory;
};
#endif SCENE_MANAGER_H