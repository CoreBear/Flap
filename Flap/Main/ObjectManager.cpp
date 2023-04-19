#pragma region Includes
#include "ObjectManager.h"

#include "Avatar.h"
#include "Consts.h"
#include "Enums.h"
#include "Food.h"
#include "NoTouchy.h"
#include "SceneObject.h"
#include "SharedGame.h"
#include "SharedRender.h"
#include "Snake.h"
#include "Structure.h"

#include <mutex>
#pragma endregion

#pragma region Initialization
ObjectManager::ObjectManager(SharedGame& _sharedGame, SharedInput& _sharedInput, SharedRender& _sharedRender) :
	mr_sharedGame(_sharedGame),
	mr_sharedRender(_sharedRender)
{
	SceneObject::AssignObjectManager(*this);

	// NOTE: Only MAX_NUMBER_OF_PLAYERS_PER_SYSTEM number of avatars
	NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE = new int[static_cast<int>(Enums::ObjectType::NumberOfTypes)] { Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM, 20, 20, 20 };

	// Generate pointers for each type
	mppp_pooledObjects = new SceneObject ** [static_cast<int>(Enums::ObjectType::NumberOfTypes)];

	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < static_cast<int>(Enums::ObjectType::NumberOfTypes); objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[objectTypeIndex];

		// Generate space for clones
		mppp_pooledObjects[objectTypeIndex] = new SceneObject * [m_numberOfObjectsPooledForThisType];

		// Generate each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			switch ((Enums::ObjectType)objectTypeIndex)
			{
			case Enums::ObjectType::Avatar:
				mppp_pooledObjects[objectTypeIndex][m_reusableIterator] = new Avatar();
				break;
			case Enums::ObjectType::Food:
				mppp_pooledObjects[objectTypeIndex][m_reusableIterator] = new Food();
				break;
			case Enums::ObjectType::NoTouchy:
				mppp_pooledObjects[objectTypeIndex][m_reusableIterator] = new NoTouchy();
				break;
			case Enums::ObjectType::Snake:
				mppp_pooledObjects[objectTypeIndex][m_reusableIterator] = new Snake();
				break;
			}
		}
	}
}
#pragma endregion

#pragma region Updates
void ObjectManager::FixedUpdate()
{
	mr_sharedRender.m_frameBufferMutex.lock();

	for (m_sceneObjectsIterator = m_sceneObjectsList.Begin(); m_sceneObjectsIterator != m_sceneObjectsList.End(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->FixedUpdate();
	}

	mr_sharedRender.m_frameBufferMutex.unlock();
}
void ObjectManager::LastUpdate()
{
	// Remove all required scene objects
	while (m_removeFromSceneObjects.empty() == false)
	{
		mp_addRemove = m_removeFromSceneObjects.front();
		m_removeFromSceneObjects.pop();

		mp_addRemove->Destroy(false);
		mp_addRemove->SetSpawnState(Enums::SpawnState::WaitingSelection);

		m_sceneObjectsList.Remove(mp_addRemove);
	}

	// Add all required scene objects
	while (m_addToSceneObjects.empty() == false)
	{
		mp_addRemove = m_addToSceneObjects.front();
		m_addToSceneObjects.pop();

		mp_addRemove->SetSpawnState(Enums::SpawnState::Spawned);
		
		m_sceneObjectsList.PushBack(mp_addRemove);
	}
}
void ObjectManager::Update()
{
	for (m_sceneObjectsIterator = m_sceneObjectsList.Begin(); m_sceneObjectsIterator != m_sceneObjectsList.End(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->Update();
	}
}
#pragma endregion

#pragma region Public Functionality
void ObjectManager::CleanScene()
{
	// Remove all scene objects waiting to be removed
	while (m_removeFromSceneObjects.empty() == false)
	{
		m_removeFromSceneObjects.front()->Destroy(true);
		m_removeFromSceneObjects.front()->SetSpawnState(Enums::SpawnState::WaitingSelection);
		m_removeFromSceneObjects.pop();
	}

	// Remove all scene objects waiting to be added
	while (m_addToSceneObjects.empty() == false)
	{
		m_addToSceneObjects.front()->SetSpawnState(Enums::SpawnState::WaitingSelection);
		m_addToSceneObjects.pop();
	}

	for (m_sceneObjectsIterator = m_sceneObjectsList.Begin(); m_sceneObjectsIterator != m_sceneObjectsList.End(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->Destroy(true);
		(*m_sceneObjectsIterator)->SetSpawnState(Enums::SpawnState::WaitingSelection);
	}

	m_sceneObjectsList.Clear();
}
void ObjectManager::Pause()
{
	for (m_sceneObjectsIterator = m_sceneObjectsList.Begin(); m_sceneObjectsIterator != m_sceneObjectsList.End(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->Pause();
	}
}
void ObjectManager::Resume()
{
	for (m_sceneObjectsIterator = m_sceneObjectsList.Begin(); m_sceneObjectsIterator != m_sceneObjectsList.End(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->Resume();
	}
}
void ObjectManager::SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2& _position, const Structure::Generic* const _genericContainer)
{
	m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[static_cast<int>(_objectType)];

	// For each pooled object of this type
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
	{
		// If object is not active
		if (mppp_pooledObjects[static_cast<int>(_objectType)][m_reusableIterator]->GetSpawenState() == Enums::SpawnState::WaitingSelection)
		{												 
			// Position and initialize it				 
			mppp_pooledObjects[static_cast<int>(_objectType)][m_reusableIterator]->SetSpawnState(Enums::SpawnState::Selected);
			mppp_pooledObjects[static_cast<int>(_objectType)][m_reusableIterator]->SetPosition(_position);
			mppp_pooledObjects[static_cast<int>(_objectType)][m_reusableIterator]->Initialize(_genericContainer);

			// Stop looking for object to spawn
			break;
		}
	}
}
void ObjectManager::Start(bool _newGame)
{
	if (_newGame)
	{

	}
	else
	{

	}
}
#pragma endregion

#pragma region Destruction
ObjectManager::~ObjectManager()
{
	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < static_cast<int>(Enums::ObjectType::NumberOfTypes); objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[objectTypeIndex];

		// For each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			delete mppp_pooledObjects[objectTypeIndex][m_reusableIterator];
		}

		delete[] mppp_pooledObjects[objectTypeIndex];
	}

	delete[] NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	delete[] mppp_pooledObjects;
}
#pragma endregion