#ifndef SHARED_COLLISION_RENDER_H
#define SHARED_COLLISION_RENDER_H

#include "DList.h"

#include <condition_variable>
#include <mutex>
#include <Windows.h>

class BufferCell;
class SceneObject;

class SharedCollisionRender final
{
private:

public:
	// Initialization
	SharedCollisionRender(const COORD& _bufferSize);
	SharedCollisionRender(const SharedCollisionRender&) = delete;
	SharedCollisionRender& operator=(const SharedCollisionRender&) = delete;

	// Member Variables
	int m_bufferSize;	// Has to be up here to initialize before other things
	bool m_threadWaitingFlag;
	BufferCell* const mp_frameBuffer;
	const COORD& SCREEN_BUFFER_CR;
	std::condition_variable m_bufferWriterIteratorConVar;
	DList<SceneObject*>::Const_Iterator m_bufferWriterIterator;
	DList<SceneObject*>::Const_Iterator m_sceneObjectsIterator;
	const DList<SceneObject*>::Const_Iterator NULL_ITERATOR;
	std::mutex m_bufferWriterIteratorMutex;

	// Destruction
	~SharedCollisionRender();
};

#endif SHARED_COLLISION_RENDER_H