#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "DList.h"
#include "Enums.h"

#include <mutex>
#include <queue>
#include <Windows.h>

namespace Structure { struct Generic; }
class SceneObject;
class SharedCollisionRender;
class SharedInput;
namespace Structure { struct Vector2; }

class ObjectManager final
{
public:
	// Initialization
	ObjectManager(SharedCollisionRender& _sharedCollisionRender, SharedInput& _sharedInput);
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

	// Updates
	void FixedUpdate();
	void LastUpdate();
	void Update();

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
	int m_numberOfObjectsPooledForThisType;
	int m_reusableIterator;
	const int* NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	std::queue<SceneObject*> m_addToSceneObjects;
	std::queue<SceneObject*> m_removeFromSceneObjects;
	SceneObject* mp_addRemove;
	SceneObject*** mppp_pooledObject;
	SharedCollisionRender& mr_sharedCollisionRender;
	std::unique_lock<std::mutex> m_bufferWriterIteratorUniqueLock;
};
#endif OBJECT_MANAGER_H