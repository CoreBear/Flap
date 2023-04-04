#ifndef COLLISION_RENDER_MANAGER_H
#define COLLISION_RENDER_MANAGER_H

#include "Structure.h"

#include <Windows.h>

class SharedMemory;

class CollisionRenderManager final
{
private:
	const int& mr_bufferSize;	// Has to be up here to initialize before other things

public:
	// Initialization
	CollisionRenderManager(const HANDLE& _outputWindowHandle, SharedMemory& _sharedMemory);
	CollisionRenderManager(const CollisionRenderManager&) = delete;
	CollisionRenderManager& operator=(const CollisionRenderManager&) = delete;

	// Functionality
	void GameUpdate();
	void OverlayUpdate();

	// Destruction
	~CollisionRenderManager();

private:
	CHAR_INFO* const mp_textBuffer;
	COORD m_topLeftCellCR;
	const HANDLE& OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;						
	SharedMemory& mr_sharedMemory;
	SMALL_RECT m_writeRegionRect;
	Structure::Vector2 m_collisionCellCR;
};

#endif COLLISION_RENDER_MANAGER_H