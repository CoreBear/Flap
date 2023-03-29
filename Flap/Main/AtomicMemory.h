#ifndef ATOMIC_MEMORY_H
#define ATOMIC_MEMORY_H

#include "Enums.h"
#include "Structure.h"

#include <condition_variable>
#include <list>
#include <mutex>

class SceneObject;

class AtomicMemory
{
public:
	// Member Variables
	char* mpp_renderBuffer;
	char* mpp_renderBufferScratch;
	char* mpp_renderBufferSwapper;
	std::condition_variable m_renderBufferConVar;
	std::mutex m_renderBufferMutex;

	// Initialization
	AtomicMemory(const Structure::Vector2<int>& _bufferSize);

	// Functionality
	void AddSpriteToScratch(const char** _sprite, int _spriteHeight, const Structure::Vector2<int>& _position);
	inline void FixedUpdateCycleCompleted() { m_renderBufferConVar.notify_one(); }
	inline Enums::ButtonPressState& GetButtonPressStateRef() { return m_spaceBarPressState; }
	inline int GetNumberOfRows() { return m_numberOfWindowRows; }
	inline std::list<SceneObject*>& GetSceneObjectsList() { return m_sceneObjectsList; }
	inline const std::list<SceneObject*>& GetSceneObjectsListForRendering () const { return m_sceneObjectsList; }
	void SwapBuffersAndClearScratch();

	// Destruction
	~AtomicMemory();

	// Member Variables
	int m_bufferSize;
	int m_numberOfWindowColumns;
	int m_numberOfWindowRows;

private:
	// Member Variables
	Enums::ButtonPressState m_spaceBarPressState;
	int m_numberOfCharactersToErase;
	int m_reusableIterator;
	std::list<SceneObject*> m_sceneObjectsList;
};

#endif ATOMIC_MEMORY_H