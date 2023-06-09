#pragma region Includes
#include "CollisionManager.h"

#include "BufferCell.h"
#include "SceneObject.h"
#include "SharedRender.h"
#include "Structure.h"
#pragma endregion

#pragma region Initialization
CollisionManager::CollisionManager(SharedRender& _sharedRender) :
	mr_sharedRender(_sharedRender),
	m_frameBufferUniqueLock(_sharedRender.m_frameBufferMutex)
{
	m_frameBufferUniqueLock.unlock();
}
#pragma endregion

#pragma region Updates
void CollisionManager::FixedUpdate()
{
	mr_sharedRender.m_frameBufferMutex.lock();

	for (m_reusableIterator = Consts::NO_VALUE; m_reusableIterator < mr_sharedRender.m_bufferSize; m_reusableIterator++)
	{
		UpdateCell(mr_sharedRender.mp_frameBuffer[m_reusableIterator]);
	}

	mr_sharedRender.m_frameBufferMutex.unlock();
	mr_sharedRender.m_frameBufferConVar.notify_one();
}
#pragma endregion

#pragma region Private Functionality
void CollisionManager::UpdateCell(BufferCell& _bufferCell) 
{	
	switch (_bufferCell.m_objectInCellIndex)
	{
	// If a collision occurred
	case _bufferCell.MAX_NUMBER_OF_OBJECTS_IN_CELL:
	{
		switch (_bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_objectType)
		{
		case Enums::ObjectType::Food:
		case Enums::ObjectType::NoTouchy:
		{
			// NOTE: Only snakes can collide with Food or NoTouchy's.
			if (_bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_objectType == Enums::ObjectType::Snake)
			{
				_bufferCell.m_character = _bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_char;
				_bufferCell.m_colorBFGround = _bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_color;

				UpdateColliders(_bufferCell);
			}
		}
		break;
		case Enums::ObjectType::Snake:
		{
			switch (_bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_objectType)
			{
				// Do nothing, Avatars are snakes
			//case Enums::ObjectType::Avatar:

			case Enums::ObjectType::Food:
			case Enums::ObjectType::NoTouchy:
			{
				_bufferCell.m_character = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_char;
				_bufferCell.m_colorBFGround = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_color;

				UpdateColliders(_bufferCell);
			}
			break;
			case Enums::ObjectType::Snake:
			{
				switch (UpdateColliders(_bufferCell))
				{
				case CollisionType::HeadOnOrSelf:
				{
					_bufferCell.m_character = Consts::EMPTY_SPACE_CHAR;
					_bufferCell.m_colorBFGround = NULL;
				}
				break;
				case CollisionType::ObjectOneSurvived:
				{
					_bufferCell.m_character = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_char;
					_bufferCell.m_colorBFGround = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_color;
				}
				break;
				case CollisionType::ObjectTwoSurvived:
				{
					_bufferCell.m_character = _bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_char;
					_bufferCell.m_colorBFGround = _bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]->m_color;
				}
				break;
				}
			}
			break;
			}
		}
		break;
		}
	}
	break;


	// If one object in cell
	case Consts::OFF_BY_ONE:
	{
		_bufferCell.m_character = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_char;
		_bufferCell.m_colorBFGround = _bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]->m_color;
	}
	break;


	// If nothing is in the cell
	default:
		break;
	}
}

CollisionManager::CollisionType CollisionManager::UpdateColliders(BufferCell& _bufferCell)
{
	mp_firstSceneObject = reinterpret_cast<SceneObject*>(_bufferCell.mp_voidSceneObject[Consts::NO_VALUE]);
	mp_secondSceneObject = reinterpret_cast<SceneObject*>(_bufferCell.mp_voidSceneObject[Consts::OFF_BY_ONE]);
	
	// If object collided with self
	if (mp_firstSceneObject == mp_secondSceneObject)
	{
		mp_firstSceneObject->Collision_IsDead(*_bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]);
		return CollisionType::HeadOnOrSelf;
	}

	// If 2 different objects collided
	else
	{
		// If first colliding object denits
		if (mp_firstSceneObject->Collision_IsDead(*_bufferCell.mp_collisionRenderInfo[SECOND_OBJECT_INDEX]))
		{
			// If second colliding object denits, else survives
			return (mp_secondSceneObject->Collision_IsDead(*_bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX])) ? CollisionType::HeadOnOrSelf : CollisionType::ObjectTwoSurvived;
		}

		// If first colliding object survives
		else
		{
			mp_secondSceneObject->Collision_IsDead(*_bufferCell.mp_collisionRenderInfo[FIRST_OBJECT_INDEX]);
			return CollisionType::ObjectOneSurvived;

		}
	}
}
#pragma endregion