#pragma region Includes
#include "Snake.h"

#include "Consts.h"
#pragma endregion

#pragma region Static Initialization
// NOTE: This value will be the ratio of the window
const Structure::Vector2<float> Snake::s_velocity(static_cast<float>(10 * Consts::FIXED_DELTA_TIME_F), static_cast<float>(7.5 * Consts::FIXED_DELTA_TIME_F));
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_currentDirection = Enums::Direction::NA;
	m_newDirection = Enums::Direction::NA;
}
Snake::Snake() : 
	// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
	SceneObject(dynamic_cast<Structure::CollisionRenderInfo*>(new Structure::SnakeCollisionRenderInfo(m_listOfBodyPositions, Enums::ObjectType::Snake, m_position))), 
	m_currentDirection(Enums::Direction::NA), 
	m_newDirection(Enums::Direction::NA), 
	m_realHeadPosition(Consts::NO_VALUE_F, Consts::NO_VALUE_F)
{
	// Create new head
	m_listOfBodyPositions.push_back(m_newBodyPosition);

	// It's also the new tail
	m_tailIterator = m_listOfBodyPositions.begin();
}
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	HandleInput();

	Move();
}
#pragma endregion

#pragma region Public Functionality
void Snake::Collision(const SceneObject& _otherCollidingObject)
{
	int h = 0;
}
#pragma endregion

#pragma region Private Functionality
void Snake::AddTail()
{
	// Create new tail
	m_listOfBodyPositions.push_back(*m_tailIterator);

	// Move iterator back to it
	++m_tailIterator;
}
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
		{
			// Change direction
			m_currentDirection = m_newDirection;
		}
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
		m_currentDirection = m_newDirection;
		break;
	}
}
void Snake::Move()
{
	// Update head's new position
	switch (m_currentDirection)
	{
	case Enums::Direction::Down:
		m_realPosition.m_y += s_velocity.m_y;
		break;
	case Enums::Direction::Left:
		m_realPosition.m_x -= s_velocity.m_x;
		break;
	case Enums::Direction::Right:
		m_realPosition.m_x += s_velocity.m_x;
		break;
	case Enums::Direction::Up:
		m_realPosition.m_y -= s_velocity.m_y;
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
	}

	// Update first (head) position
	(*m_headTraversingIterator).m_x = m_realPosition.m_x;
	(*m_headTraversingIterator).m_y = m_realPosition.m_y;

	m_position.m_x = static_cast<int>(m_realHeadPosition.m_x);
	m_position.m_y = static_cast<int>(m_realHeadPosition.m_y);
}
#pragma endregion