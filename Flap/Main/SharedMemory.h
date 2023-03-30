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
	std::condition_variable m_renderIteratorConVar;
	std::list<SceneObject*>::iterator m_renderIterator;	

	// Functionality
	inline std::mutex* GetInputQueueMutexPtr() { return m_inputQueueMutex; }
	inline std::mutex& GetInputQueueMutexRef(int _inputQueueIndex) { return m_inputQueueMutex[_inputQueueIndex]; }
	inline std::queue<Structure::Input>* GetInputQueuePtr() { return m_inputQueue; }
	inline std::queue<Structure::Input>& GetInputQueueRef(int _inputQueueIndex) { return m_inputQueue[_inputQueueIndex]; }
	inline const std::list<SceneObject*>::const_iterator& GetNullIteratorRef() { return m_nullIterator; }
	inline std::list<SceneObject*>::iterator& GetSceneObjectsIteratorRef() { return m_sceneObjectsIterator; }
	inline std::mutex& GetSpriteWriteInIteratorMutexRef() { return m_renderIteratorMutex; }
	inline std::list<SceneObject*>::iterator& GetSpriteWriteInIteratorRef() { return m_renderIterator; }
	inline void SetNullIterator(const std::list<SceneObject*>::const_iterator& _nullIterator) { m_nullIterator = _nullIterator; }

private:
	// Member Variables
	std::list<SceneObject*>::const_iterator m_nullIterator;		
	std::list<SceneObject*>::iterator m_sceneObjectsIterator;
	std::mutex m_renderIteratorMutex;
	std::mutex m_inputQueueMutex[Consts::MAX_NUMBER_OF_PLAYERS];
	std::queue<Structure::Input> m_inputQueue[Consts::MAX_NUMBER_OF_PLAYERS];
};

#endif ATOMIC_MEMORY_H