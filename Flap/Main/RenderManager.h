#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Manager.h"
#include "SceneObject.h"

#include <list>
#include <Windows.h>

class AtomicMemory;

class RenderManager : public Manager
{
public:
	// Initialization
	RenderManager(AtomicMemory& _atomicMemory, HANDLE& _windowHandle);

	// Updates
	void Update() override;

	// Destruction
	~RenderManager() override;

private:
	// Member Variables
	AtomicMemory* mp_atomicMemory;
	CHAR_INFO* mp_textBuffer;
	COORD m_screenBufferCR;
	COORD m_topLeftCellCR;
	HANDLE* mp_windowHandle;
	int m_reusableIterator;
	std::list<SceneObject*>::const_iterator m_sceneObjectsIterator;
	const SceneObject::SpriteInfo* mp_spriteInfo;
	SMALL_RECT m_writeRegionRect;

	// Functionality
	void WriteSpritesIntoBuffer(const std::list<SceneObject*>& _sceneObjectsList);
};

#endif RENDER_MANAGER_H