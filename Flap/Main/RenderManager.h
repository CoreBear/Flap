#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "GameThreadBase.h"
#include "Structure.h"

#include <mutex>
#include <Windows.h>

class SharedRender;

class RenderManager final : public GameThreadBase
{
public:
	// Initialization
	RenderManager(const HANDLE& _outputWindowHandle, SharedRender& _sharedRender);
	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;

	// Updates
	void Update() override;

	// Destruction
	~RenderManager() override;

private:
	CHAR_INFO* const mp_renderBuffer;
	COORD m_topLeftCellCR;
	const HANDLE& OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;						
	SharedRender& mr_sharedRender;
	SMALL_RECT m_writeRegionRect;
	std::unique_lock<std::mutex> m_frameBufferUniqueLock;
};

#endif RENDER_MANAGER_H