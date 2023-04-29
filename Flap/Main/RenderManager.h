#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "GameThreadBase.h"
#include "Structure.h"

#include <mutex>
#include <Windows.h>

class SharedGame;

class RenderManager final : public GameThreadBase
{
public:
	// Initialization
	RenderManager(const HANDLE& _outputWindowHandle, SharedGame& _sharedGame);
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~RenderManager() override;

private:
	CHAR_INFO* const mp_renderBuffer;
	COORD FRAME_BUFFER_HEIGHT_WIDTH;
	COORD m_frameBufferTopLeftPosition;
	const HANDLE OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;						
	SharedGame& mr_sharedGame;
	SMALL_RECT m_drawRegion;
	std::unique_lock<std::mutex> m_frameBufferUniqueLock;
};

#endif RENDER_MANAGER_H