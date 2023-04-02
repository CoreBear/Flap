#pragma region Includes
#include "Snake.h"

#include "Consts.h"
#include "SceneManager.h"
#include "SharedMemory.h"
#pragma endregion

#pragma region Static Initialization
SharedMemory* Snake::sp_sharedMemory = nullptr;
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_currentDirection = Enums::Direction::NA;
	m_newDirection = Enums::Direction::NA;

	// Causes snake to check for first input, each frame, at the start
	m_numberOfFramesPerCell = static_cast<unsigned int>(Consts::OFF_BY_ONE);
	m_moveTargetFrame = SceneManager::s_fixedFrameCount + m_numberOfFramesPerCell;

	UpdateMoveSpeed(_genericContainer.m_int);

	(*m_listOfBodyPositions.begin()) = m_position;

	mp_collisionRenderInfo->m_character = static_cast<char>(_genericContainer.m_int2 + Consts::ASCII_OFFSET);
}
Snake::Snake() : 
	// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
	SceneObject(dynamic_cast<Structure::CollisionRenderInfo*>(new Structure::SnakeCollisionRenderInfo(m_listOfBodyPositions, Enums::ObjectType::Snake, m_position))), 
	m_currentDirection(Enums::Direction::NA), 
	m_newDirection(Enums::Direction::NA), 
	m_numberOfTailSectionsToAdd(Consts::NO_VALUE)
{
	// Create head
	m_listOfBodyPositions.push_back(m_newTailPosition);

	// It's also the new tail
	m_tailIterator = m_listOfBodyPositions.begin();
}
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	// If movement frame
	if (SceneManager::s_fixedFrameCount == m_moveTargetFrame)
	{
		// Reset movement target
		m_moveTargetFrame = SceneManager::s_fixedFrameCount + m_numberOfFramesPerCell;

		TryAddTail();

		TryTurn();

		Move();
	}
}
#pragma endregion

#pragma region Public Functionality
void Snake::Collision(const SceneObject& _otherCollidingObject, const Structure::Vector2& _collisionCellCR)
{
	// If didn't collide with self
	if (&_otherCollidingObject != this)
	{
		OTHER_COLLISION_PACKAGE = &_otherCollidingObject.GetCollisionPackageRef();

		// Eating food
		if (OTHER_COLLISION_PACKAGE->m_objectType == Enums::ObjectType::Food)
		{
			m_numberOfTailSectionsToAdd = OTHER_COLLISION_PACKAGE->m_int;

			m_newTailPosition = *m_tailIterator;
		}

		// Collided with another snake
		else
		{
			// If this snake collided with its head
			if (m_position == _collisionCellCR)
			{
				Death();
			}

			// If this snake did not collide with its head
			else
			{
				// Increase this player's points
			}
		}
	}

	// If collided with self
	else
	{
		Death();
	}
}
#pragma endregion

#pragma region Private Functionality
void Snake::Death()
{
	// Game over
	int h = 0;
}
void Snake::HorizontalTurn()
{
	// Store speed type
	m_numberOfFramesPerCell = m_numberOfFramesPerCellHorizontal;

	Turn();
}
void Snake::Move()
{
	// Update head's new position
	switch (m_currentDirection)
	{
	case Enums::Direction::Down:
	{
		// Bind to screen or die (this needs to be off by one'd)
		if (m_position.m_y + Consts::OFF_BY_ONE < sp_sharedMemory->SCREEN_BUFFER_CR.Y)
		{
			++m_position.m_y;
		}
		else
		{
			Death();
		}
	}
		break;
	case Enums::Direction::Left:
	{
		// Bind to screen or die
		if (m_position.m_x - Consts::OFF_BY_ONE > -Consts::OFF_BY_ONE)
		{
			--m_position.m_x;
		}
		else
		{
			Death();
		}
	}
		break;
	case Enums::Direction::Right:
	{
		// Bind to screen or die (this needs to be off by one'd)
		if (m_position.m_x + Consts::OFF_BY_ONE < sp_sharedMemory->SCREEN_BUFFER_CR.X)
		{
			++m_position.m_x;
		}
		else
		{
			Death();
		}
	}
		break;
	case Enums::Direction::Up:
	{
		// Bind to screen or die
		if (m_position.m_y - Consts::OFF_BY_ONE > -Consts::OFF_BY_ONE)
		{
			--m_position.m_y;
		}
		else
		{
			Death();
		}
	}
		break;

		// NOTE: Notice the snake will not move due to the return
	default:
		return;
	}

	m_headTraversingIterator = m_tailIterator;

	// If snake's size is greater than 1
	if (m_headTraversingIterator != m_listOfBodyPositions.begin())
	{
		// Move iterator down
		--m_headTraversingIterator;

		// Move the snake from the tail to the position right before the head
		for (m_tailTraversingIterator = m_tailIterator; m_headTraversingIterator != m_listOfBodyPositions.begin(); --m_headTraversingIterator, --m_tailTraversingIterator)
		{
			// Move the position closer to the tail, to the position closer to the head
			(*m_tailTraversingIterator) = (*m_headTraversingIterator);
		}

		// Move the position closer to the tail, to the position closer to the head
		(*m_tailTraversingIterator) = (*m_headTraversingIterator);
	}

	// Update first (head) position
	(*m_headTraversingIterator) = m_position;
}
void Snake::TryAddTail()
{
	// If a tail should be added
	if (m_numberOfTailSectionsToAdd > Consts::NO_VALUE)
	{
		// Add tail
		m_listOfBodyPositions.push_back(m_newTailPosition);

		// Move iterator to this new tail
		++m_tailIterator;

		// Account for this tail's addition
		--m_numberOfTailSectionsToAdd;

		// Set new tail position, in-case another tail needs to be added
		m_newTailPosition = *m_tailIterator;
	}
}
void Snake::TryTurn()
{
	switch (m_currentDirection)
	{
		// Currently moving vertical
	case Enums::Direction::Down:
	case Enums::Direction::Up:
	{
		// If new direction is horizontal
		switch (m_newDirection)
		{
		case Enums::Direction::Left:
		case Enums::Direction::Right:
			HorizontalTurn();
			break;
		}
	}
	break;

	// Currently moving horizontal
	case Enums::Direction::Left:
	case Enums::Direction::Right:
	{
		// If new direction is vertical
		switch (m_newDirection)
		{
		case Enums::Direction::Down:
		case Enums::Direction::Up:
			VerticalTurn();
			break;
		}
	}
	break;

	// If not currently moving (game just started), change direction
	default:
	{
		switch (m_newDirection)
		{
		case Enums::Direction::Down:
		case Enums::Direction::Up:
			VerticalTurn();
			break;
		case Enums::Direction::Left:
		case Enums::Direction::Right:
			HorizontalTurn();
			break;

			// NOTE: No direction has been pressed
		default:
			break;
		}
	}
	break;
	}
}
void Snake::Turn()
{
	// Change direction
	m_currentDirection = m_newDirection;

	// Set speed
	m_moveTargetFrame = SceneManager::s_fixedFrameCount + m_numberOfFramesPerCell;
}
void Snake::UpdateMoveSpeed(int _speed)
{
	m_numberOfFramesPerCellVertical = static_cast<unsigned int>(Consts::FPS_TARGET / _speed);

	// HACK: Hardcoded...
	m_numberOfFramesPerCellHorizontal = static_cast<unsigned int>(m_numberOfFramesPerCellVertical);// / 3.7F);
}
void Snake::VerticalTurn()
{
	// Store speed type
	m_numberOfFramesPerCell = m_numberOfFramesPerCellVertical;

	Turn();
}
#pragma endregion