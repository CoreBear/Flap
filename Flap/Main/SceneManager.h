#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Manager.h"

#include <chrono>

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
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	ObjectManager* mp_objectManager;
	OverlayManager* mp_overlayManager;
	SceneType m_sceneType;
	SharedMemory* mp_sharedMemory;
};
#endif SCENE_MANAGER_H