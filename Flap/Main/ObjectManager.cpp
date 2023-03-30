#pragma region Includes
#include "ObjectManager.h"

#include "Bird.h"
#include "Consts.h"
#include "SceneObject.h"
#include "SharedMemory.h"
#pragma endregion

#pragma region Initialization
ObjectManager::ObjectManager(SharedMemory& _sharedMemory) :
	mr_sceneObjectsFixedUpdateIterator(_sharedMemory.GetSceneObjectsIteratorRef()), 
	mp_sharedMemory(&_sharedMemory),
	m_spriteWriteInIteratorUniqueLock(_sharedMemory.m_spriteWriteInIteratorMutex)
{
	SceneObject::AssignObjectManager(*this);

	mp_numberOfObjectsToPoolPerType = new int[static_cast<int>(Enums::ObjectType::NumberOfTypes)] { 20 };

	// Generate pointers for each type
	mpp_pooledObject = new SceneObject ** [static_cast<int>(Enums::ObjectType::NumberOfTypes)];

	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < (int)Enums::ObjectType::NumberOfTypes; objectTypeIndex++)
	{
		m_numberOfObjectsPooledForThisType = mp_numberOfObjectsToPoolPerType[objectTypeIndex];

		// Generate space for clones
		mpp_pooledObject[objectTypeIndex] = new SceneObject * [m_numberOfObjectsPooledForThisType];

		// Generate each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			switch ((Enums::ObjectType)objectTypeIndex)
			{
			case Enums::ObjectType::Bird:
				mpp_pooledObject[objectTypeIndex][m_reusableIterator] = new Bird();
				break;
			}
		}
	}

	// Set pointers to the beginning of the container
	mr_sceneObjectsFixedUpdateIterator = m_sceneObjectsList.end();
	mp_sharedMemory->m_spriteWriteInIterator = m_sceneObjectsList.end();
	mp_sharedMemory->m_nullIterator = m_sceneObjectsList.end();

	// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
	m_spriteWriteInIteratorUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void ObjectManager::FixedUpdate() 
{
	if (m_sceneObjectsList.empty() == false)
	{
		m_spriteWriteInIteratorUniqueLock.lock();

		// If Render thread is already waiting, release the Scene thread
		if (mp_sharedMemory->m_threadWaitingFlag)
		{
			mp_sharedMemory->m_spriteWriteInIteratorConVar.notify_one();
		}

		// If Render thread is not already waiting, flip flag and wait
		else
		{
			mp_sharedMemory->m_threadWaitingFlag = true;

			// When Render thread releases this (Scene) thread, flip flag
			mp_sharedMemory->m_spriteWriteInIteratorConVar.wait(m_spriteWriteInIteratorUniqueLock);

			mp_sharedMemory->m_threadWaitingFlag = false;
		}

		// Reset iterators
		mr_sceneObjectsFixedUpdateIterator = m_sceneObjectsList.begin();
		mp_sharedMemory->m_spriteWriteInIterator = m_sceneObjectsList.begin();

		// NOTE/WARNING: IMPORTANT TO UNLOCK!!!
		m_spriteWriteInIteratorUniqueLock.unlock();

		// NOTE: Initialization is done above, while within mutex
		for (; mr_sceneObjectsFixedUpdateIterator != m_sceneObjectsList.end(); ++mr_sceneObjectsFixedUpdateIterator)
		{
			(*mr_sceneObjectsFixedUpdateIterator)->FixedUpdate();
		}
	}
}
void ObjectManager::LastUpdate()
{
	// Remove all required scene objects
	while (m_removeFromSceneObjects.empty() == false)
	{
		m_sceneObjectsList.remove(m_removeFromSceneObjects.front());
		m_removeFromSceneObjects.pop();
	}

	// Add all required scene objects
	while (m_addToSceneObjects.empty() == false)
	{
		m_sceneObjectsList.push_back(m_addToSceneObjects.front());
		m_addToSceneObjects.pop();
	}
}
void ObjectManager::Update()
{
	for (m_sceneObjectsOtherUpdatesIterator = m_sceneObjectsList.begin(); m_sceneObjectsOtherUpdatesIterator != m_sceneObjectsList.end(); ++m_sceneObjectsOtherUpdatesIterator)
	{
		(*m_sceneObjectsOtherUpdatesIterator)->Update();
	}
}
#pragma endregion

#pragma region Public Functionality
void ObjectManager::SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2<int>& _position, const Structure::Generic& _genericContainer)
{
	m_numberOfObjectsPooledForThisType = mp_numberOfObjectsToPoolPerType[static_cast<int>(_objectType)];

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
		m_numberOfObjectsPooledForThisType = mp_numberOfObjectsToPoolPerType[objectTypeIndex];

		// For each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsPooledForThisType; m_reusableIterator++)
		{
			delete mpp_pooledObject[objectTypeIndex][m_reusableIterator];
		}

		delete[] mpp_pooledObject[objectTypeIndex];
	}

	delete[] mp_numberOfObjectsToPoolPerType;
	delete[] mpp_pooledObject;
}
#pragma endregion