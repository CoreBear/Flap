#pragma region Includes
#include "Food.h"

#include "BufferCell.h"
#include "Consts.h"
#pragma endregion

#pragma region Initialization
Food::Food()
{
	m_collisionRenderInfo.m_objectType = Enums::ObjectType::Food;

	// HACK: Do this else where
	m_color = Consts::WHITE_BACKGROUND;
}
void Food::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_collisionRenderInfo.m_color = m_color;
	m_collisionRenderInfo.m_position = m_position;

	UpdateValue(_genericContainer->m_int);
}
#pragma endregion

#pragma region Private Functionality
void Food::UpdateValue(int _value)
{
	m_collisionRenderInfo.m_value = _value;

	m_collisionRenderInfo.m_char = static_cast<char>(_value + Consts::ASCII_OFFSET);
}
#pragma endregion