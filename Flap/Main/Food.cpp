#pragma region Includes
#include "Food.h"

#include "Consts.h"
#pragma endregion

#pragma region Initialization
// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
Food::Food() : SceneObject(new Structure::RenderInfo(Enums::ObjectType::Food, m_position))
{
	m_collisionPackage.m_objectType = Enums::ObjectType::Food;
}
void Food::Initialize(const Structure::Generic* const _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	UpdateValue(_genericContainer->m_int);
}
#pragma endregion

#pragma region Public Functionality
void Food::Collision(const SceneObject& _otherCollidingObject, const Structure::Vector2& _collisionCellCR)
{
	Denitialize();
}
#pragma endregion

#pragma region Private Functionality
void Food::UpdateValue(int _value)
{
	m_collisionPackage.m_int = _value;

	mp_renderInfo->m_character = static_cast<char>(_value + Consts::ASCII_OFFSET);
}
#pragma endregion