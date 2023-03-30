#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Manager.h"
#include "Structure.h"

#include <list>
#include <mutex>
#include <Windows.h>

class SharedMemory;
class SceneObject;

class RenderManager final : public Manager
{
public:
	// Initialization
	RenderManager(HANDLE& _windowHandle, SharedMemory& _sharedMemory, const Structure::Vector2<int>& _bufferSize);

	// Updates
	void Update() override;

	// Destruction
	~RenderManager() override;

private:
	// Member Variables
	bool m_frameWritingIsComplete;
	bool m_writeSpritesIntoBuffer;
	char* mpp_renderBufferForRendering;
	char* mpp_renderBufferForWriting;
	char* mpp_renderBufferSwapper;
	CHAR_INFO* mp_textBuffer;
	COORD m_screenBufferCR;
	COORD m_topLeftCellCR;
	HANDLE* mp_windowHandle;
	int m_bufferSize;
	int m_numberOfCharactersToErase;
	int m_reusableIterator;
	const std::list<SceneObject*>::const_iterator& mr_nullIterator;
	std::list<Structure::SpriteInfo::BodyNode*>::const_iterator m_bodyNodeIterator;
	SharedMemory* mp_sharedMemory;
	SMALL_RECT m_writeRegionRect;
	std::unique_lock<std::mutex> m_renderIteratorUniqueLock;
	Structure::Vector2<int> m_nodePosition;

	// Functionality
	void WriteConnectorsIntoBuffer(bool _positive, int& _dimenstionToUpdate);
	void WriteIntoBuffer();
	void WriteSpriteIntoBuffer(const Structure::SpriteInfo& _spriteInfo);
};

#endif RENDER_MANAGER_H