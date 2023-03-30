#pragma region Includes
#include "Snake.h"

#include "Consts.h"
#pragma endregion

#pragma region Static Initialization
const float Snake::s_moveSpeed = static_cast<float>(10 * Consts::FIXED_DELTA_TIME_F);
#pragma endregion

#pragma region Initialization
void Snake::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	// HACK: Randomize this
	m_moveDirection = Enums::Direction::Down;
}
#pragma endregion

#pragma region Updates
void Snake::FixedUpdate()
{
	switch (m_moveDirection)
	{
	case Enums::Direction::Down:
	{
		m_headPosition.m_y += s_moveSpeed;
		m_spriteInfo.m_bodyNodes.front()->m_position.m_y = static_cast<int>(m_headPosition.m_y);
	}
	break;
	case Enums::Direction::Left:
	{
		m_headPosition.m_x -= s_moveSpeed;
		m_spriteInfo.m_bodyNodes.front()->m_position.m_x = static_cast<int>(m_headPosition.m_x);
	}
	break;
	case Enums::Direction::Right:
	{
		m_headPosition.m_x += s_moveSpeed;
		m_spriteInfo.m_bodyNodes.front()->m_position.m_x = static_cast<int>(m_headPosition.m_x);
	}
	break;
	case Enums::Direction::Up:
	{
		m_headPosition.m_y -= s_moveSpeed;
		m_spriteInfo.m_bodyNodes.front()->m_position.m_y = static_cast<int>(m_headPosition.m_y);
	}
	break;
	}
}
#pragma endregion