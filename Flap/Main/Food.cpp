#pragma region Includes
#include "Food.h"

#include "Consts.h"
#pragma endregion

#pragma region Initialization
void Food::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_collisionRenderInfo.m_colorIndex = static_cast<int>(Enums::Color::White);
	m_collisionRenderInfo.m_color = Consts::BACKGROUND_COLORS[m_collisionRenderInfo.m_colorIndex] | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::Black)];
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