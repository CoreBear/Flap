#ifndef COLLISION_RENDER_BUFFER_TO_OBJECT_AND_SCREEN_H
#define COLLISION_RENDER_BUFFER_TO_OBJECT_AND_SCREEN_H

#include "Structure.h"

#include <Windows.h>

class SharedCollisionRender;

class CollisionRenderBufferToObjectAndScreen final
{
public:
	// Initialization
	CollisionRenderBufferToObjectAndScreen(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender);
	CollisionRenderBufferToObjectAndScreen(const CollisionRenderBufferToObjectAndScreen&) = delete;
	CollisionRenderBufferToObjectAndScreen& operator=(const CollisionRenderBufferToObjectAndScreen&) = delete;

	// Functionality
	void GameUpdate();
	void OverlayUpdate();

	// Destruction
	~CollisionRenderBufferToObjectAndScreen();

private:
	CHAR_INFO* const mp_textBuffer;
	COORD m_topLeftCellCR;
	const HANDLE& OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;						
	SharedCollisionRender& mr_sharedCollisionRender;
	SMALL_RECT m_writeRegionRect;
	Structure::Vector2 m_collisionCellCR;
};

#endif COLLISION_RENDER_BUFFER_TO_OBJECT_AND_SCREEN_H