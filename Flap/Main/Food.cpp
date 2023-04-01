#pragma region Includes
#include "Food.h"
#pragma endregion

#pragma region Initialization
// NOTE/WARNING: Allocated memory is destroyed in the SceneObject destructor
Food::Food() : SceneObject(new Structure::CollisionRenderInfo(Enums::ObjectType::Food, m_position), new Structure::Generic)
{
	mp_collisionPackage->m_objectType = Enums::ObjectType::Food;
}
void Food::Initialize(const Structure::Generic& _genericContainer)
{
	SceneObject::Initialize(_genericContainer);

	UpdateValue(_genericContainer.m_int);
}
#pragma endregion

#pragma region Public Functionality
void Food::Collision(const SceneObject& _otherCollidingObject)
{
	Denitialize();
}
#pragma endregion

#pragma region Private Functionality
void Food::UpdateValue(int _value)
{
	mp_collisionPackage->m_int = _value;

	constexpr int ASCII_OFFSET = static_cast<int>('0');
	mp_collisionRenderInfo->m_character = static_cast<char>(_value + ASCII_OFFSET);
}
#pragma endregion