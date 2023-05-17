#pragma region Includes
#include "Snake.h"

#include "Consts.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "SharedGame.h"
#include "Structure.h"
#include "Tools.h"
#pragma endregion

#pragma region Static Initialization
Structure::Generic Snake::s_genericContainer;
int Snake::s_snakeStartingSpeed = 10;			// HACK: Hardcoded
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_currentDirection = Enums::InputName::NA;
	m_newDirection = Enums::InputName::NA;

	m_newCollisionRenderInfo.m_colorIndex = sp_sharedGame->GetPlayerSnakeColorIndex(_genericContainer->m_int - Consts::OFF_BY_ONE);
	m_newCollisionRenderInfo.m_objectType = Enums::ObjectType::Snake;
	m_newCollisionRenderInfo.m_color = Consts::BACKGROUND_COLORS[m_newCollisionRenderInfo.m_colorIndex];
	m_newCollisionRenderInfo.m_position = m_position;
	
	m_bodyNodes.PushBack(m_newCollisionRenderInfo);

	// If not numbering the snake's body
	if (_genericContainer->m_bool == false)
	{
		m_newCollisionRenderInfo.m_char = Consts::EMPTY_SPACE_CHAR;
	}

	m_length = Consts::OFF_BY_ONE;

	m_currentSpeed = s_snakeStartingSpeed;

	m_numberOfTailSectionsToAdd = Consts::NO_VALUE;

	Resume();
}
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	if (GameManager::s_masterFixedFrameCount == m_moveTargetFrame)
	{
		m_moveTargetFrame = GameManager::s_masterFixedFrameCount + m_numberOfFramesBetweenMoves;

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
	else
	{
		switch (_collisionRenderInfo.m_objectType)
		{
			// Eating food
		case Enums::ObjectType::Food:
		{
			m_numberOfTailSectionsToAdd = _collisionRenderInfo.m_value;

			m_newTailPosition = (*m_bodyNodes.GetTail()).m_position;

			++m_currentSpeed;
			m_numberOfFramesBetweenMoves = static_cast<unsigned int>(Consts::FPS_TARGET / m_currentSpeed);
		}
		break;

		// Collided with another snake
		case Enums::ObjectType::Snake:
		{
			// If this snake's head didn't collide
			if (m_position != _collisionRenderInfo.m_position)
			{
				// Increase this player's points, because other snake's head collided
			}

			// NOTE: If this snake's head collided, fallthrough
		}

		// NOTE: Notice the fallthrough
		case Enums::ObjectType::NoTouchy:
			Death();
			return true;
		}
	}

	return false;
}
void Snake::Pause()
{
	m_numberOfFramesLeftBeforePause = m_moveTargetFrame - GameManager::s_masterFixedFrameCount;

	if (m_numberOfFramesLeftBeforePause < 3)
	{
		m_numberOfFramesLeftBeforePause;
	}
}
void Snake::Resume()
{
	m_numberOfFramesBetweenMoves = static_cast<unsigned int>(Consts::FPS_TARGET / m_currentSpeed);
	m_moveTargetFrame = GameManager::s_masterFixedFrameCount + m_numberOfFramesBetweenMoves;
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
		// NOTE: This does not account for ties. Lol
		if (m_length > sp_sharedGame->m_largestSnakeLengthUponDeath)
		{
			sp_sharedGame->m_largestSnakeLengthUponDeath = m_length;
			sp_sharedGame->m_largestSnakeColor = m_bodyNodes.Peek().m_color;
		}

		// Start at the second node (if it exists)
		m_headTraversingIterator = m_bodyNodes.Begin();
		++m_headTraversingIterator;

		// Begin destorying snake
		Denitialize();

		constexpr int EVERY_OTHER_NODE = 2;
		// For every other body node, spawn food
		for (/*Initialization occurs above*/; m_headTraversingIterator != m_bodyNodes.End(); m_headTraversingIterator += EVERY_OTHER_NODE)
		{
			// HACK: Give the food different values
			s_genericContainer.m_int = 1;
			sp_objectManager->SpawnObject(Enums::ObjectType::Food, (*m_headTraversingIterator).m_position, &s_genericContainer);
		}
	}

	// If there are no more snakes (GAME OVER)
	else
	{
		sp_sharedGame->m_gameActivityIndexMutex.lock();
		sp_sharedGame->m_gameActivityIndex = Enums::GameActivity::ExitToResults;
		sp_sharedGame->m_gameActivityIndexMutex.unlock();


		if (sp_sharedGame->GetSinglePlayerBool())
		{
			sp_sharedGame->m_largestSnakeLengthUponDeath = m_length;
			sp_sharedGame->m_largestSnakeColor = m_bodyNodes.Peek().m_color;
		}
		else
		{
			// NOTE: This does not account for ties. Lol
			if (m_length > sp_sharedGame->m_largestSnakeLengthUponDeath)
			{
				sp_sharedGame->m_largestSnakeLengthUponDeath = m_length;
				sp_sharedGame->m_largestSnakeColor = m_bodyNodes.Peek().m_color;
			}
		}
	}
}
void Snake::Move()
{
	// Update head's new position
	switch (m_currentDirection)
	{
	case Enums::InputName::Down:
	{
		++m_position.m_y;
	}
	break;
	case Enums::InputName::Left:
	{
		--m_position.m_x;
	}
	break;
	case Enums::InputName::Right:
	{
		++m_position.m_x;
	}
	break;
	case Enums::InputName::Up:
	{
		--m_position.m_y;
	}
	break;

	// NOTE: Notice the snake will not move due to the return
	default:
		return;
	}

	sp_sharedGame->RemoveAvailableSpawnIndex(m_position.m_x, m_position.m_y);

	m_headTraversingIterator = m_bodyNodes.GetTail();

	sp_sharedGame->AddAvailableSpawnIndex((*m_headTraversingIterator).m_position.m_x, (*m_headTraversingIterator).m_position.m_y);

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

		sp_sharedGame->RemoveAvailableSpawnIndex(m_newTailPosition.m_x, m_newTailPosition.m_y);

		// Add tail
		m_bodyNodes.PushBack(m_newCollisionRenderInfo);

		++m_length;

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
void Snake::Destroy(bool _cleanScene)
{
	if (_cleanScene == false)
	{
		for (m_headTraversingIterator = m_bodyNodes.Begin(); m_headTraversingIterator != m_bodyNodes.End(); ++m_headTraversingIterator)
		{
			sp_sharedGame->AddAvailableSpawnIndex((*m_headTraversingIterator).m_position.m_x, (*m_headTraversingIterator).m_position.m_y);
		}
	}

	m_bodyNodes.Clear();
}
#pragma endregion