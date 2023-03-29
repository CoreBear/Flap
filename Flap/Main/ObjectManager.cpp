#pragma region Includes
#include "ObjectManager.h"

#include "AtomicMemory.h"
#include "Bird.h"
#include "Consts.h"
#include "SceneObject.h"
#pragma endregion

#pragma region Initialization
ObjectManager::ObjectManager(AtomicMemory& _atomicMemory) : mr_sceneObjectsList(_atomicMemory.GetSceneObjectsListRef())
{
	SceneObject::AssignObjectManager(*this);

	mp_numberOfObjectsToPoolPerType = new int[static_cast<int>(Enums::ObjectType::NumberOfTypes)] { 10 };

	// Generate pointers for each type
	mpp_pooledObject = new SceneObject ** [static_cast<int>(Enums::ObjectType::NumberOfTypes)];

	// For each object type
	for (int objectTypeIndex = Consts::NO_VALUE; objectTypeIndex < (int)Enums::ObjectType::NumberOfTypes; objectTypeIndex++)
	{
		m_numberOfObjectsToPoolForThisType = mp_numberOfObjectsToPoolPerType[objectTypeIndex];

		// Generate space for clones
		mpp_pooledObject[objectTypeIndex] = new SceneObject * [m_numberOfObjectsToPoolForThisType];

		// Generate each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsToPoolForThisType; m_reusableIterator++)
		{
			switch ((Enums::ObjectType)objectTypeIndex)
			{
			case Enums::ObjectType::Bird:
				mpp_pooledObject[objectTypeIndex][m_reusableIterator] = new Bird();
				break;
			}
		}
	}
}
#pragma endregion

#pragma region Updates
void ObjectManager::FixedUpdate() 
{
	for (m_sceneObjectsIterator = mr_sceneObjectsList.begin(); m_sceneObjectsIterator != mr_sceneObjectsList.end(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->FixedUpdate();
	}
}
void ObjectManager::LastUpdate()
{
	// Remove all required scene objects
	while (m_removeFromSceneObjects.empty() == false)
	{
		mr_sceneObjectsList.remove(m_removeFromSceneObjects.front());
		m_removeFromSceneObjects.pop();
	}

	// Add all required scene objects
	while (m_addToSceneObjects.empty() == false)
	{
		mr_sceneObjectsList.push_back(m_addToSceneObjects.front());
		m_addToSceneObjects.pop();
	}
}
void ObjectManager::Update()
{
	for (m_sceneObjectsIterator = mr_sceneObjectsList.begin(); m_sceneObjectsIterator != mr_sceneObjectsList.end(); ++m_sceneObjectsIterator)
	{
		(*m_sceneObjectsIterator)->Update();
	}
}
#pragma endregion

#pragma region Public Functionality
void ObjectManager::SpawnObject(Enums::ObjectType _objectType, const Structure::Vector2<int>& _position, const Structure::Generic& _genericContainer)
{
	m_numberOfObjectsToPoolForThisType = mp_numberOfObjectsToPoolPerType[static_cast<int>(_objectType)];

	// For each pooled object of this type
	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsToPoolForThisType; m_reusableIterator++)
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
		m_numberOfObjectsToPoolForThisType = mp_numberOfObjectsToPoolPerType[objectTypeIndex];

		// For each clone
		for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < m_numberOfObjectsToPoolForThisType; m_reusableIterator++)
		{
			delete mpp_pooledObject[objectTypeIndex][m_reusableIterator];
		}

		delete[] mpp_pooledObject[objectTypeIndex];
	}

	delete[] mp_numberOfObjectsToPoolPerType;
	delete[] mpp_pooledObject;
}
#pragma endregion