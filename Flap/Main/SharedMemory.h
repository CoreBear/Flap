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
	std::condition_variable m_renderIteratorConVar;
	DList<SceneObject*>::Const_Iterator m_renderIterator;
	const DList<SceneObject*>::Const_Iterator NULL_ITERATOR;

	// Functionality
	inline std::mutex* const GetInputQueueMutexPtr() { return m_inputQueueMutex; }
	inline std::mutex& GetInputQueueMutexRef(int _inputQueueIndex) { return m_inputQueueMutex[_inputQueueIndex]; }
	inline std::queue<Structure::Input>* const GetInputQueuePtr() { return m_inputQueue; }
	inline std::queue<Structure::Input>& GetInputQueueRef(int _inputQueueIndex) { return m_inputQueue[_inputQueueIndex]; }
	inline std::mutex& GetRenderIteratorMutexRef() { return m_renderIteratorMutex; }
	inline DList<SceneObject*>::Const_Iterator& GetSceneObjectsIteratorRef() { return m_sceneObjectsIterator; }

private:
	// Member Variables
	DList<SceneObject*>::Const_Iterator m_sceneObjectsIterator;
	std::mutex m_inputQueueMutex[Consts::MAX_NUMBER_OF_PLAYERS];
	std::mutex m_renderIteratorMutex;
	std::queue<Structure::Input> m_inputQueue[Consts::MAX_NUMBER_OF_PLAYERS];
};

#endif ATOMIC_MEMORY_H