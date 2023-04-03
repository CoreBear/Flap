#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "DList.h"
#include "Enums.h"
#include "Manager.h"
#include "Structure.h"

#include <mutex>
#include <queue>
#include <Windows.h>

class SceneObject;
class SharedMemory;

class ObjectManager final : public Manager
{
public:
	// Initialization
	ObjectManager(SharedMemory& _sharedMemory);
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

	// Updates
	void FixedUpdate();
	void LastUpdate();
	void Update() override;

	// Functionality
	inline void AddToSceneObjects(SceneObject* const _sceneObject) { m_addToSceneObjects.push(_sceneObject); }
	inline void RemoveFromSceneObjects(SceneObject* const _sceneObject) { m_removeFromSceneObjects.push(_sceneObject); }
	void SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2& _position, const Structure::Generic* const _genericContainer = nullptr);

	// Desturction
	~ObjectManager();

private:
	// Member Variables
	DList<SceneObject*> m_sceneObjectsList;
	DList<SceneObject*>::Const_Iterator m_sceneObjectsOtherUpdatesIterator;
	DList<SceneObject*>::Const_Iterator& mr_sceneObjectsFixedUpdateIterator;
	int m_numberOfObjectsPooledForThisType;
	int m_reusableIterator;
	const int* NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	std::queue<SceneObject*> m_addToSceneObjects;
	std::queue<SceneObject*> m_removeFromSceneObjects;
	SceneObject*** mpp_pooledObject;
	SharedMemory* const mp_sharedMemory;
	std::unique_lock<std::mutex> m_renderIteratorUniqueLock;
};
#endif OBJECT_MANAGER_H