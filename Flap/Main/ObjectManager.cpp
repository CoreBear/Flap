#pragma region Includes
#include "ObjectManager.h"

#include "Avatar.h"
#include "Consts.h"
#include "Enums.h"
#include "Food.h"
#include "SceneObject.h"
#include "SharedCollisionRender.h"
#include "SharedGame.h"
#include "Snake.h"
#include "Structure.h"
#pragma endregion

#pragma region Initialization
ObjectManager::ObjectManager(SharedCollisionRender& _sharedCollisionRender, SharedGame& _sharedGame, SharedInput& _sharedInput) :
	mr_sharedCollisionRender(_sharedCollisionRender),
	mr_sharedGame(_sharedGame),
	m_bufferWriterIteratorUniqueLock(_sharedCollisionRender.m_bufferWriterIteratorMutex)
{
	SceneObject::AssignObjectManager(*this);

	// NOTE: Only MAX_NUMBER_OF_PLAYERS_PER_SYSTEM number of avatars
	NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE = new int[static_cast<int>(Enums::ObjectType::NumberOfTypes)] { Consts::MAX_NUMBER_OF_PLAYERS_PER_SYSTEM, 20, 20 };

	// Generate pointers for each type
	mppp_pooledObject = new SceneObject ** [static_cast<int>(Enums::ObjectType::NumberOfTypes)];

	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < static_cast<int>(Enums::ObjectType::NumberOfTypes); objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[objectTypeIndex];

		// Generate space for clones
		mppp_pooledObject[objectTypeIndex] = new SceneObject * [m_numberOfObjectsPooledForThisType];

		// Generate each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			switch ((Enums::ObjectType)objectTypeIndex)
			{
			case Enums::ObjectType::Avatar:
				mppp_pooledObject[objectTypeIndex][m_reusableIterator] = new Avatar(_sharedInput);
				break;
			case Enums::ObjectType::Food:
				mppp_pooledObject[objectTypeIndex][m_reusableIterator] = new Food();
				break;
			case Enums::ObjectType::Snake:
				mppp_pooledObject[objectTypeIndex][m_reusableIterator] = new Snake();
				break;
			}
		}
	}

	// Set pointers to the beginning of the container
	mr_sharedCollisionRender.m_bufferWriterIterator = m_sceneObjectsList.End();
	mr_sharedCollisionRender.m_sceneObjectsIterator = m_sceneObjectsList.End();

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_bufferWriterIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void ObjectManager::FixedUpdate() 
{
	if (m_sceneObjectsList.IsEmpty() == false)
	{
		m_bufferWriterIteratorUniqueLock.lock();

		// If Buffer thread is already waiting
		if (mr_sharedCollisionRender.m_threadWaitingFlag)
		{
			// Release it
			mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();

			// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
			mr_sharedCollisionRender.m_threadWaitingFlag = false;
		}

		// If Buffer thread is not already waiting
		else
		{
			// Notify system that this (Scene) thread is waiting
			mr_sharedCollisionRender.m_threadWaitingFlag = true;

			// When Buffer thread releases this (Scene) thread
			mr_sharedCollisionRender.m_bufferWriterIteratorConVar.wait(m_bufferWriterIteratorUniqueLock);

			// Let it know it can release, because this (Scene) thread needs to go first
			mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();
		}

		// Reset iterators
		mr_sharedCollisionRender.m_bufferWriterIterator = m_sceneObjectsList.Begin();
		mr_sharedCollisionRender.m_sceneObjectsIterator = m_sceneObjectsList.Begin();

		m_bufferWriterIteratorUniqueLock.unlock();

		for (/*Initialization is done above, while within mutex*/; mr_sharedCollisionRender.m_sceneObjectsIterator != m_sceneObjectsList.End(); ++mr_sharedCollisionRender.m_sceneObjectsIterator)
		{
			(*mr_sharedCollisionRender.m_sceneObjectsIterator)->FixedUpdate();
		}

		mr_sharedGame.m_gameStateMutex.lock();
		if (mr_sharedGame.m_gameState == Enums::GameState::ExitToResults)
		{
			mr_sharedGame.m_gameStateMutex.unlock();
			return;
		}
		mr_sharedGame.m_gameStateMutex.unlock();

		m_bufferWriterIteratorUniqueLock.lock();

		// If Buffer thread is already waiting
		if (mr_sharedCollisionRender.m_threadWaitingFlag)
		{
			// Release it
			mr_sharedCollisionRender.m_bufferWriterIteratorConVar.notify_one();

			// Notify the system that nothing is waiting (when this is unlocked and the Buffer thread grabs the lock)
			mr_sharedCollisionRender.m_threadWaitingFlag = false;
		}

		// If Buffer thread is not already waiting
		else
		{
			// Notify system that this (Scene) thread is waiting
			mr_sharedCollisionRender.m_threadWaitingFlag = true;

			// When Render thread releases this (Scene) thread
			mr_sharedCollisionRender.m_bufferWriterIteratorConVar.wait(m_bufferWriterIteratorUniqueLock);
		}

		m_bufferWriterIteratorUniqueLock.unlock();
	}
}
void ObjectManager::LastUpdate()
{
	// Remove all required scene objects
	while (m_removeFromSceneObjects.empty() == false)
	{
		mp_addRemove = m_removeFromSceneObjects.front();
		m_sceneObjectsList.Remove(mp_addRemove);
		m_removeFromSceneObjects.pop();
		mp_addRemove->SetSpawnState(Enums::SpawnState::WaitingSelection);
	}

	// Add all required scene objects
	while (m_addToSceneObjects.empty() == false)
	{
		mp_addRemove = m_addToSceneObjects.front();
		m_sceneObjectsList.PushBack(mp_addRemove);
		m_addToSceneObjects.pop();
		mp_addRemove->SetSpawnState(Enums::SpawnState::Spawned);
	}
}
void ObjectManager::Update()
{
	for (m_sceneObjectsOtherUpdatesIterator = m_sceneObjectsList.Begin(); m_sceneObjectsOtherUpdatesIterator != m_sceneObjectsList.End(); ++m_sceneObjectsOtherUpdatesIterator)
	{
		(*m_sceneObjectsOtherUpdatesIterator)->Update();
	}
}
#pragma endregion

#pragma region Public Functionality
void ObjectManager::CleanScene()
{
	// Empty container
	while (m_removeFromSceneObjects.empty() == false)
	{
		m_removeFromSceneObjects.pop();
	}

	// Empty container
	while (m_addToSceneObjects.empty() == false)
	{
		m_addToSceneObjects.pop();
	}
	
	for (mr_sharedCollisionRender.m_sceneObjectsIterator = m_sceneObjectsList.Begin(); mr_sharedCollisionRender.m_sceneObjectsIterator != m_sceneObjectsList.End(); ++mr_sharedCollisionRender.m_sceneObjectsIterator)
	{
		(*mr_sharedCollisionRender.m_sceneObjectsIterator)->Denitialize(false);
	}

	m_sceneObjectsList.Clear();
}
void ObjectManager::SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2& _position, const Structure::Generic* const _genericContainer)
{
	m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[static_cast<int>(_objectType)];

	// For each pooled object of this type
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
	{
		// If object is not active
		if (mppp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->GetSpawenState() == Enums::SpawnState::WaitingSelection)
		{												 
			// Position and initialize it				 
			mppp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->SetSpawnState(Enums::SpawnState::Selected);
			mppp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->SetPosition(_position);
			mppp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->Initialize(_genericContainer);

			// Stop looking for object to spawn
			break;
		}
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
			delete mppp_pooledObject[objectTypeIndex][m_reusableIterator];
		}

		delete[] mppp_pooledObject[objectTypeIndex];
	}

	delete[] NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	delete[] mppp_pooledObject;
}
#pragma endregion