#ifndef COLLISION_RENDER_READ_OUT_OF_BUFFER_H
#define COLLISION_RENDER_READ_OUT_OF_BUFFER_H

#include "Structure.h"

#include <Windows.h>

class SharedCollisionRender;

class CollisionRenderReadOutOfBuffer final
{
public:
	// Initialization
	CollisionRenderReadOutOfBuffer(const HANDLE& _outputWindowHandle, SharedCollisionRender& _sharedCollisionRender);
	CollisionRenderReadOutOfBuffer(const CollisionRenderReadOutOfBuffer&) = delete;
	CollisionRenderReadOutOfBuffer& operator=(const CollisionRenderReadOutOfBuffer&) = delete;

	// Updates
	void FixedUpdate();

	// Destruction
	~CollisionRenderReadOutOfBuffer();

private:
	CHAR_INFO* const mp_textBuffer;
	COORD m_topLeftCellCR;
	const HANDLE& OUTPUT_WINDOW_HANDLE;
	int m_reusableIterator;						
	SharedCollisionRender& mr_sharedCollisionRender;
	SMALL_RECT m_writeRegionRect;
	Structure::Vector2 m_collisionCellCR;
};

#endif COLLISION_RENDER_READ_OUT_OF_BUFFER_H