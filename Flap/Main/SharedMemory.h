#ifndef ATOMIC_MEMORY_H
#define ATOMIC_MEMORY_H

#include "Structure.h"

#include <list>
#include <mutex>
#include <queue>

class SceneObject;

class SharedMemory final
{
public:
	// Member Variables
	std::list<SceneObject*>::iterator m_spriteWriteInIterator;	
	std::mutex m_spriteWriteInIteratorMutex;
	std::list<SceneObject*>::const_iterator m_nullIterator;		// Cannot be a reference, because SharedMemory is created before the container this points to

	// Functionality
	inline std::queue<Structure::Input>& GetInputQueueRef() { return m_inputQueue; }
	inline std::list<SceneObject*>::iterator& GetSceneObjectsIteratorRef() { return m_sceneObjectsIterator; }
	inline std::list<SceneObject*>::iterator& GetSpriteWriteInIteratorRef() { return m_spriteWriteInIterator; }

private:
	// Member Variables
	std::list<SceneObject*>::iterator m_sceneObjectsIterator;
	std::queue<Structure::Input> m_inputQueue;
};

#endif ATOMIC_MEMORY_H