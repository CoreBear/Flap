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
	// Member Variables
	CHAR_INFO* const mp_renderBuffer;
	COORD FRAME_BUFFER_HEIGHT_WIDTH;
	COORD m_frameBufferTopLeftPosition;
	const HANDLE OUTPUT_WINDOW_HANDLE;
	int m_renderBufferIndex;
	int m_reusableIterator_1;
	int m_reusableIterator_2;
	SharedGame& mr_sharedGame;
	SMALL_RECT m_drawRegion;
	std::unique_lock<std::mutex> m_frameBufferUniqueLock;
	Structure::Vector2<short> m_cellCR;
};

#endif RENDER_MANAGER_H