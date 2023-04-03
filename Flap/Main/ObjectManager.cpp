#pragma region Includes
#include "ObjectManager.h"

#include "Avatar.h"
#include "Consts.h"
#include "Food.h"
#include "SceneObject.h"
#include "SharedMemory.h"
#include "Snake.h"
#pragma endregion

#pragma region Initialization
ObjectManager::ObjectManager(SharedMemory& _sharedMemory) :
	mr_sceneObjectsFixedUpdateIterator(_sharedMemory.GetSceneObjectsIteratorRef()), 
	mp_sharedMemory(&_sharedMemory),
	m_collisionRenderIteratorUniqueLock(_sharedMemory.GetCollisionRenderIteratorMutexRef())
{
	SceneObject::AssignObjectManager(*this);

	// NOTE: Only MAX_NUMBER_OF_PLAYERS number of avatars
	NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE = new int[static_cast<int>(Enums::ObjectType::NumberOfTypes)] { Consts::MAX_NUMBER_OF_PLAYERS, 20, 20 };

	// Generate pointers for each type
	mpp_pooledObject = new SceneObject ** [static_cast<int>(Enums::ObjectType::NumberOfTypes)];

	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < (int)Enums::ObjectType::NumberOfTypes; objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[objectTypeIndex];

		// Generate space for clones
		mpp_pooledObject[objectTypeIndex] = new SceneObject * [m_numberOfObjectsPooledForThisType];

		// Generate each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			switch ((Enums::ObjectType)objectTypeIndex)
			{
			case Enums::ObjectType::Avatar:
				mpp_pooledObject[objectTypeIndex][m_reusableIterator] = new Avatar(_sharedMemory);
				break;
			case Enums::ObjectType::Food:
				mpp_pooledObject[objectTypeIndex][m_reusableIterator] = new Food();
				break;
			case Enums::ObjectType::Snake:
				mpp_pooledObject[objectTypeIndex][m_reusableIterator] = new Snake();
				break;
			}
		}
	}

	// Set pointers to the beginning of the container
	mr_sceneObjectsFixedUpdateIterator = m_sceneObjectsList.End();
	mp_sharedMemory->m_collisionRenderIterator = m_sceneObjectsList.End();
	mp_sharedMemory->SetNullIterator(m_sceneObjectsList.End());

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_collisionRenderIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void ObjectManager::FixedUpdate() 
{
	if (m_sceneObjectsList.IsEmpty() == false)
	{
		m_collisionRenderIteratorUniqueLock.lock();

		// If CollisionRender thread is already waiting, release the this (Scene) thread
		if (mp_sharedMemory->m_threadWaitingFlag)
		{
			mp_sharedMemory->m_collisionRenderIteratorConVar.notify_one();
		}

		// If CollisionRender thread is not already waiting, flip flag and wait
		else
		{
			mp_sharedMemory->m_threadWaitingFlag = true;

			// When CollisionRender thread releases this (Scene) thread, flip flag
			mp_sharedMemory->m_collisionRenderIteratorConVar.wait(m_collisionRenderIteratorUniqueLock);

			// Let it know it can release, because this (Scene) thread needs to go first
			mp_sharedMemory->m_collisionRenderIteratorConVar.notify_one();

			mp_sharedMemory->m_threadWaitingFlag = false;
		}

		// Reset iterators
		mr_sceneObjectsFixedUpdateIterator = m_sceneObjectsList.Begin();
		mp_sharedMemory->m_collisionRenderIterator = m_sceneObjectsList.Begin();

		// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
		m_collisionRenderIteratorUniqueLock.unlock();

		for (/*Initialization is done above, while within mutex*/; mr_sceneObjectsFixedUpdateIterator != m_sceneObjectsList.End(); ++mr_sceneObjectsFixedUpdateIterator)
		{
			m_collisionRenderIteratorUniqueLock.lock();
			(*mr_sceneObjectsFixedUpdateIterator)->FixedUpdate();
			m_collisionRenderIteratorUniqueLock.unlock();
		}
	}
}
void ObjectManager::LastUpdate()
{
	// Remove all required scene objects
	while (m_removeFromSceneObjects.empty() == false)
	{
		m_sceneObjectsList.Remove(m_removeFromSceneObjects.front());
		m_removeFromSceneObjects.pop();
	}

	// Add all required scene objects
	while (m_addToSceneObjects.empty() == false)
	{
		m_sceneObjectsList.PushBack(m_addToSceneObjects.front());
		m_addToSceneObjects.pop();
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
void ObjectManager::SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2& _position, const Structure::Generic& _genericContainer)
{
	m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[static_cast<int>(_objectType)];

	// For each pooled object of this type
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
	{
		// If object is not active
		if (mpp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->IsActive() == false)
		{												 
			// Position and initialize it				 
			mpp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->SetPosition(_position);
			mpp_pooledObject[static_cast<int>(_objectType)][m_reusableIterator]->Initialize(_genericContainer);

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
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < (int)Enums::ObjectType::NumberOfTypes; objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE[objectTypeIndex];

		// For each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			delete mpp_pooledObject[objectTypeIndex][m_reusableIterator];
		}

		delete[] mpp_pooledObject[objectTypeIndex];
	}

	delete[] NUMBER_OF_OBJECTS_TO_POOL_PER_TYPE;
	delete[] mpp_pooledObject;
}
#pragma endregion