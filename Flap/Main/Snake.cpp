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
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	HandleTurn();

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
void Snake::HandleTurn()
{
	if (m_currentDirection != m_newDirection)
	{
		switch (m_currentDirection)
		{
		case Enums::Direction::Down:
		case Enums::Direction::Up:
		{
			// If direction change
			switch (m_newDirection)
			{
			case Enums::Direction::Left:
			case Enums::Direction::Right:
			{
				m_currentDirection = m_newDirection;

				//m_collisionRenderInfo.AddTail();
			}
			break;
			}
		}
		break;
		case Enums::Direction::Left:
		case Enums::Direction::Right:
		{
			// If direction change
			switch (m_newDirection)
			{
			case Enums::Direction::Down:
			case Enums::Direction::Up:
			{
				m_currentDirection = m_newDirection;

				//m_collisionRenderInfo.AddTail();
			}
			break;
			}
		}
		break;
		default:
			m_currentDirection = m_newDirection;
			break;
		}
	}
}
void Snake::Move()
{
	switch (m_currentDirection)
	{
	case Enums::Direction::Down:
	{
		m_realPosition.m_y += s_velocity.m_y;

		m_collisionRenderInfo.UpdatePosition(m_realPosition);
	}
	break;
	case Enums::Direction::Left:
	{
		m_realPosition.m_x -= s_velocity.m_x;

		m_collisionRenderInfo.UpdatePosition(m_realPosition);
	}
	break;
	case Enums::Direction::Right:
	{
		m_realPosition.m_x += s_velocity.m_x;

		m_collisionRenderInfo.UpdatePosition(m_realPosition);
	}
	break;
	case Enums::Direction::Up:
	{
		m_realPosition.m_y -= s_velocity.m_y;

		m_collisionRenderInfo.UpdatePosition(m_realPosition);
	}
	break;
	default:
		break;
	}
}
#pragma endregion