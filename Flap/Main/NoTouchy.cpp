#pragma region Includes
#include "NoTouchy.h"

#include "Structure.h"
#pragma endregion

#pragma region Initialization
void NoTouchy::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	m_collisionRenderInfo.m_position = m_position;
}
NoTouchy::NoTouchy()
{
	m_collisionRenderInfo.m_char = 'X';
	m_collisionRenderInfo.m_colorIndex = static_cast<int>(Enums::Color::Red);
	m_collisionRenderInfo.m_objectType = Enums::ObjectType::NoTouchy;
	m_collisionRenderInfo.m_color = Consts::BACKGROUND_COLORS[m_collisionRenderInfo.m_colorIndex] | Consts::FOREGROUND_COLORS[static_cast<int>(Enums::Color::Black)];
}
#pragma endregion