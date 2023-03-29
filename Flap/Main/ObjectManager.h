#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "Enums.h"
#include "Manager.h"
#include "Structure.h"

#include <list>
#include <queue>
#include <Windows.h>

class AtomicMemory;
class SceneObject;

class ObjectManager : public Manager
{
public:
	// Initialization
	ObjectManager(AtomicMemory& _atomicMemory);

	// Updates
	void FixedUpdate();
	void LastUpdate();
	void Update() override;

	// Functionality
	inline void AddToSceneObjects(SceneObject* _sceneObject) { m_addToSceneObjects.push(_sceneObject); }
	inline void RemoveFromSceneObjects(SceneObject* _sceneObject) { m_removeFromSceneObjects.push(_sceneObject); }
	void SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2<int>& _position, const Structure::Generic& _genericContainer);

	// Desturction
	~ObjectManager();

private:
	// Member Variables
	int m_numberOfObjectsToPoolForThisType;
	int m_reusableIterator;
	int* mp_numberOfObjectsToPoolPerType;

	// HACK: Initialize with values
	std::list<SceneObject*>& mr_sceneObjectsList;
	std::list<SceneObject*>::iterator m_sceneObjectsIterator;

	// HACK: Initialize with values
	std::queue<SceneObject*> m_addToSceneObjects;
	std::queue<SceneObject*> m_removeFromSceneObjects;
	SceneObject*** mpp_pooledObject;
};
#endif OBJECT_MANAGER_H