#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "DList.h"
#include "Enums.h"

#include <queue>
#include <Windows.h>

namespace Structure { struct Generic; }
class SceneObject;
class SharedGame;
class SharedInput;
class SharedRender;
namespace Structure { template<class Type> struct Vector2; }

class ObjectManager final
{
public:
	// Initialization
	ObjectManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender);
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

	// Updates
	void FixedUpdate();
	void LastUpdate();
	void Update();

	// Functionality
	inline void AddToSceneObjects(SceneObject* const _sceneObject) { m_addToSceneObjects.push(_sceneObject); }
	void CleanScene();
	inline void RemoveFromSceneObjects(SceneObject* const _sceneObject) { m_removeFromSceneObjects.push(_sceneObject); }
	void Pause();
	void Resume();
	void SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2<int>& _position, const Structure::Generic* const _genericContainer = nullptr);
	void Start(bool _newGame);

	// Desturction
	~ObjectManager();

private:
	// Member Variables
	DList<SceneObject*> m_sceneObjectsList;
	DList<SceneObject*>::Const_Iterator m_sceneObjectsIterator;
	int m_numberOfObjectsPooledForThisType;
	int m_reusableIterator;
	const int* NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	std::queue<SceneObject*> m_addToSceneObjects;
	std::queue<SceneObject*> m_removeFromSceneObjects;
	SceneObject* mp_addRemove;
	SceneObject*** mppp_pooledObjects;
	SharedGame& mr_sharedGame;
	SharedRender& mr_sharedRender;
};
#endif OBJECT_MANAGER_H