#ifndef ATOMIC_MEMORY_H
#define ATOMIC_MEMORY_H

#include "Consts.h"
#include "Structure.h"

#include <condition_variable>
#include <list>
#include <mutex>
#include <queue>

class SceneObject;

class SharedMemory final
{
public:
	// Initialization
	SharedMemory();

	// Member Variables
	bool m_threadWaitingFlag;
	std::condition_variable m_spriteWriteInIteratorConVar;
	std::list<SceneObject*>::const_iterator m_nullIterator;		// Cannot be a reference, because SharedMemory is created before the container this points to
	std::list<SceneObject*>::iterator m_spriteWriteInIterator;	
	std::mutex m_spriteWriteInIteratorMutex;

	// Functionality
	inline std::mutex* GetInputQueueMutexPtr() { return m_inputQueueMutex; }
	inline std::mutex& GetInputQueueMutexRef(int _inputQueueIndex) { return m_inputQueueMutex[_inputQueueIndex]; }
	inline std::queue<Structure::Input>* GetInputQueuePtr() { return m_inputQueue; }
	inline std::queue<Structure::Input>& GetInputQueueRef(int _inputQueueIndex) { return m_inputQueue[_inputQueueIndex]; }
	inline std::list<SceneObject*>::iterator& GetSceneObjectsIteratorRef() { return m_sceneObjectsIterator; }
	inline std::list<SceneObject*>::iterator& GetSpriteWriteInIteratorRef() { return m_spriteWriteInIterator; }

private:
	// Member Variables
	std::list<SceneObject*>::iterator m_sceneObjectsIterator;
	std::mutex m_inputQueueMutex[Consts::MAX_NUMBER_OF_PLAYERS];
	std::queue<Structure::Input> m_inputQueue[Consts::MAX_NUMBER_OF_PLAYERS];
};

#endif ATOMIC_MEMORY_H