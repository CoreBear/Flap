#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Manager.h"

#include <chrono>

class AtomicMemory;
class ObjectManager;
class OverlayManager;

class SceneManager : public Manager
{
public:
	// Static Variables
	static unsigned int s_simFrameCount;

	// Initialization
	SceneManager(AtomicMemory& _atomicMemory);

	// Updates
	void Update() override;

	// Destruction
	~SceneManager() override;

private:
	// Member Variables
	AtomicMemory* mp_atomicMemory;
	enum class SceneType { Game, Overlay };
	std::chrono::high_resolution_clock::time_point m_currentTime, m_lastTime;
	ObjectManager* mp_objectManager;
	OverlayManager* mp_overlayManager;
	SceneType m_sceneType;
};
#endif SCENE_MANAGER_H