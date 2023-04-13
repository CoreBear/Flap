#pragma region Includes
#include "Snake.h"

#include "BufferCell.h"
#include "Consts.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#include "SharedRender.h"
#include "Structure.h"
#pragma endregion

#pragma region Static Initialization
bool Snake::s_moveThisFrame = false;
Structure::Generic Snake::s_genericContainer;
SharedGame* Snake::sp_sharedGame = nullptr;
unsigned int Snake::s_numberOfFramesBetweenMoves;
unsigned int Snake::s_numberOfFramesLeftBeforePause;
unsigned int Snake::s_snakeMoveTargetFrame;
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_currentDirection = Enums::InputName::NA;
	m_newDirection = Enums::InputName::NA;

	constexpr int NUMBER_TO_CHAR_OFFSET = static_cast<int>('0');
	m_newCollisionRenderInfo.m_char = static_cast<char>(_genericContainer->m_int + NUMBER_TO_CHAR_OFFSET);
	m_newCollisionRenderInfo.m_objectType = Enums::ObjectType::Snake;
	m_newCollisionRenderInfo.m_color = Consts::BACKGROUND_COLORS[sp_sharedGame->GetPlayerSnakeColorIndexRef(_genericContainer->m_int - Consts::OFF_BY_ONE)];
	m_newCollisionRenderInfo.m_position = m_position;
	
	m_bodyNodes.PushBack(m_newCollisionRenderInfo);

	// Reset so the body doesn't have these numbers
	m_newCollisionRenderInfo.m_char = Consts::EMPTY_SPACE_CHAR;

	m_numberOfTailSectionsToAdd = Consts::NO_VALUE;
}
Snake::Snake() : 
	m_currentDirection(Enums::InputName::NA), 
	m_newDirection(Enums::InputName::NA), 
	m_numberOfTailSectionsToAdd(Consts::NO_VALUE)
{
	return;
}
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	// If movement frame
	if (s_moveThisFrame)
	{
		TryAddTail();

		TryTurn();

		Move();
	}

	WriteIntoFrameBuffer();
}
#pragma endregion

#pragma region Public Functionality
bool Snake::Collision_IsDead(const Structure::CollisionRenderInfo& _collisionRenderInfo, bool _collidedWithSelf)
{
	if (_collidedWithSelf)
	{
		Death();
		return true;
	}
	// NOTE: If execution makes it beyond this point, snake is not colliding with itself

	// Eating food
	if (_collisionRenderInfo.m_objectType == Enums::ObjectType::Food)
	{
		m_numberOfTailSectionsToAdd = _collisionRenderInfo.m_value;

		m_newTailPosition = (*m_bodyNodes.GetTail()).m_position;
	}

	// Collided with another snake
	else
	{
		// If this snake's head collided
		if (m_position == _collisionRenderInfo.m_position)
		{
			Death();
			return true;
		}

		// If this snake's head didn't collide
		else
		{
			// Increase this player's points, because other snake's head collided
		}
	}

	return false;
}
#pragma endregion

#pragma region Private Functionality
void Snake::Death()
{
	// On less snake
	sp_sharedGame->DecrementNumberOfSnakesInGame();

	// If there are more snakes
	if (sp_sharedGame->GetNumberOfSnakesInGame() != Consts::NO_VALUE)
	{
		// Start at the second node (if it exists)
		m_headTraversingIterator = m_bodyNodes.Begin();
		++m_headTraversingIterator;

		constexpr int EVERY_OTHER_NODE = 2;

		// Denitialize snake
		Denitialize(true);

		// For every other body node, spawn food
		for (/*Initialization occurs above*/; m_headTraversingIterator != m_bodyNodes.End(); m_headTraversingIterator += EVERY_OTHER_NODE)
		{
			// HACK: Give the food different values
			s_genericContainer.m_int = 1;
			sp_objectManager->SpawnObject(Enums::ObjectType::Food, (*m_headTraversingIterator).m_position, &s_genericContainer);
		}
	}

	// If there are no more snakes
	else
	{
		sp_sharedGame->m_gameStateMutex.lock();
		sp_sharedGame->m_gameState = Enums::GameState::ExitToResults;
		sp_sharedGame->m_gameStateMutex.unlock();
	}
}
void Snake::Move()
{
	// Update head's new position
	switch (m_currentDirection)
	{
	case Enums::InputName::Down:
		++m_position.m_y;
		break;
	case Enums::InputName::Left:
		--m_position.m_x;
		break;
	case Enums::InputName::Right:
		++m_position.m_x;
		break;
	case Enums::InputName::Up:
		--m_position.m_y;
		break;

		// NOTE: Notice the snake will not move due to the return
	default:
		return;
	}

	m_headTraversingIterator = m_bodyNodes.GetTail();


	// If snake's size is greater than 1
	if (m_headTraversingIterator != m_bodyNodes.Begin())
	{
		// Move iterator down
		--m_headTraversingIterator;

		// Move the snake from the tail to the position right before the head
		for (m_tailTraversingIterator = m_bodyNodes.GetTail(); m_headTraversingIterator != m_bodyNodes.Begin(); --m_headTraversingIterator, --m_tailTraversingIterator)
		{
			// Move the position closer to the tail, to the position closer to the head
			(*m_tailTraversingIterator).m_position = (*m_headTraversingIterator).m_position;
		}

		// Move the position closer to the tail, to the position closer to the head
		(*m_tailTraversingIterator).m_position = (*m_headTraversingIterator).m_position;
	}

	// Update first (head) position
	(*m_headTraversingIterator).m_position = m_position;
}
void Snake::TryAddTail()
{
	// If a tail should be added
	if (m_numberOfTailSectionsToAdd > Consts::NO_VALUE)
	{
		m_newCollisionRenderInfo.m_position = m_newTailPosition;

		// Add tail
		m_bodyNodes.PushBack(m_newCollisionRenderInfo);

		// Account for this tail's addition
		--m_numberOfTailSectionsToAdd;
	}
}
void Snake::TryTurn()
{
	switch (m_currentDirection)
	{
		// Currently moving vertical
	case Enums::InputName::Down:
	case Enums::InputName::Up:
	{
		// If new direction is horizontal
		switch (m_newDirection)
		{
		case Enums::InputName::Left:
		case Enums::InputName::Right:
		{
			// Change direction
			m_currentDirection = m_newDirection;
		}
		break;
		}
	}
	break;

	// Currently moving horizontal
	case Enums::InputName::Left:
	case Enums::InputName::Right:
	{
		// If new direction is vertical
		switch (m_newDirection)
		{
		case Enums::InputName::Down:
		case Enums::InputName::Up:
		{
			// Change direction
			m_currentDirection = m_newDirection;
		}
		break;
		}
	}
	break;

	// If not currently moving (game just started), change direction
	default:
	{
		switch (m_newDirection)
		{
		case Enums::InputName::Down:
		case Enums::InputName::Up:
		case Enums::InputName::Left:
		case Enums::InputName::Right:
		{
			// Change direction
			m_currentDirection = m_newDirection;
		}
		break;

		// NOTE: No direction has been pressed
		default:
			break;
		}
	}
	break;
	}
}
void Snake::WriteIntoFrameBuffer()
{
	m_headTraversingIterator = m_bodyNodes.Begin();

	// If new head position is valid
	if (CheckPositionValidity((*m_headTraversingIterator).m_position) == true)
	{
		// Write each body node into the frame buffer 
		for (/*Initialized above*/; m_headTraversingIterator != m_bodyNodes.End(); ++m_headTraversingIterator)
		{
			WriteIntoFrameBufferCell((*m_headTraversingIterator));
		}
	}

	// If new head position is invalid
	else
	{
		Death();
	}
}
#pragma endregion

#pragma region Destruction
void Snake::Denitialize(bool _removeFromSceneObjects)
{
	m_bodyNodes.Clear();

	SceneObject::Denitialize(_removeFromSceneObjects);
}
#pragma endregion