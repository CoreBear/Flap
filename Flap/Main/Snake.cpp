#pragma region Includes
#include "Snake.h"

#include "Consts.h"
#include "SceneManager.h"
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_currentDirection = Enums::Direction::NA;
	m_newDirection = Enums::Direction::NA;

	UpdateMoveSpeed(_genericContainer.m_int);
}
Snake::Snake() : 
	// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
	SceneObject(dynamic_cast<Structure::CollisionRenderInfo*>(new Structure::SnakeCollisionRenderInfo(m_listOfBodyPositions, Enums::ObjectType::Snake, m_position)), nullptr), 
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
	HandleInput();

	// If movement frame
	if (SceneManager::s_fixedFrameCount == m_moveTargetFrame)
	{
		// Reset movement target
		m_moveTargetFrame = SceneManager::s_fixedFrameCount + m_numberOfFramesPerCell;

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
			m_newTailPosition.m_x = m_tailIterator->m_x;
			m_newTailPosition.m_y = m_tailIterator->m_y;
		}

		Move();
	}
}
#pragma endregion

#pragma region Public Functionality
void Snake::Collision(const SceneObject& _otherCollidingObject)
{
	mp_collisionPackage = _otherCollidingObject.GetCollisionPackagePtr();

	if (mp_collisionPackage != nullptr)
	{
		if (mp_collisionPackage->m_objectType == Enums::ObjectType::Food)
		{
			m_numberOfTailSectionsToAdd = mp_collisionPackage->m_int;
			
			m_newTailPosition.m_x = m_tailIterator->m_x;
			m_newTailPosition.m_y = m_tailIterator->m_y;
		}
	}
}
#pragma endregion

#pragma region Private Functionality
void Snake::HandleInput()
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
		++m_position.m_y;
		break;
	case Enums::Direction::Left:
		--m_position.m_x;
		break;
	case Enums::Direction::Right:
		++m_position.m_x;
		break;
	case Enums::Direction::Up:
		--m_position.m_y;
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
			(*m_tailTraversingIterator).m_x = (*m_headTraversingIterator).m_x;
			(*m_tailTraversingIterator).m_y = (*m_headTraversingIterator).m_y;
		}

		// Move the position closer to the tail, to the position closer to the head
		(*m_tailTraversingIterator).m_x = (*m_headTraversingIterator).m_x;
		(*m_tailTraversingIterator).m_y = (*m_headTraversingIterator).m_y;
	}

	// Update first (head) position
	(*m_headTraversingIterator).m_x = m_position.m_x;
	(*m_headTraversingIterator).m_y = m_position.m_y;
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