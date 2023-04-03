#ifndef ATOMIC_MEMORY_H
#define ATOMIC_MEMORY_H

#include "Consts.h"
#include "DList.h"
#include "Structure.h"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <Windows.h>

class SceneObject;

class SharedMemory final
{
public:
	// Initialization
	SharedMemory(const COORD& _bufferSize);
	SharedMemory(const SharedMemory&) = delete;
	SharedMemory& operator=(const SharedMemory&) = delete;

	// Member Variables
	bool m_threadWaitingFlag;
	const COORD& SCREEN_BUFFER_CR;
	std::condition_variable m_collisionRenderIteratorConVar;
	DList<SceneObject*>::Const_Iterator m_collisionRenderIterator;

	// Functionality
	inline std::mutex* GetInputQueueMutexPtr() const { return m_inputQueueMutex; }
	inline std::mutex& GetInputQueueMutexRef(int _inputQueueIndex) { return m_inputQueueMutex[_inputQueueIndex]; }
	inline std::queue<Structure::Input>* GetInputQueuePtr() { return m_inputQueue; }
	inline std::queue<Structure::Input>& GetInputQueueRef(int _inputQueueIndex) { return m_inputQueue[_inputQueueIndex]; }
	inline const DList<SceneObject*>::Const_Iterator& GetNullIteratorRef() { return m_nullIterator; }
	inline DList<SceneObject*>::Const_Iterator& GetSceneObjectsIteratorRef() { return m_sceneObjectsIterator; }
	inline std::mutex& GetCollisionRenderIteratorMutexRef() { return m_collisionRenderIteratorMutex; }
	inline void SetNullIterator(const DList<SceneObject*>::Const_Iterator& _nullIterator) { m_nullIterator = _nullIterator; }

private:
	// Member Variables
	DList<SceneObject*>::Const_Iterator m_sceneObjectsIterator;
	DList<SceneObject*>::Const_Iterator m_nullIterator;
	std::mutex m_collisionRenderIteratorMutex;
	std::mutex m_inputQueueMutex[Consts::MAX_NUMBER_OF_PLAYERS];
	std::queue<Structure::Input> m_inputQueue[Consts::MAX_NUMBER_OF_PLAYERS];
};

#endif ATOMIC_MEMORY_H