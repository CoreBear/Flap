#pragma region Includes
#include "SceneObject.h"

#include "ObjectManager.h"
#pragma endregion

#pragma region Static Initialization
ObjectManager* SceneObject::sp_objectManager = nullptr;

SceneObject::SceneObject()
{
	m_isActive = false;
}
#pragma endregion

#pragma region Initialization
void SceneObject::Initialize(const Structure::Generic& _genericContainer)
{
	m_isActive = true;

	sp_objectManager->AddToSceneObjects(this);
}
#pragma endregion

#pragma region Public Functionality
void SceneObject::SetPosition(const Structure::Vector2<int>& _position)
{
	m_position.m_x = static_cast<float>(_position.m_x);
	m_position.m_y = static_cast<float>(_position.m_y);

	m_spriteInfo.m_bodyNodes.front()->m_position.m_x = static_cast<int>(m_position.m_x);
	m_spriteInfo.m_bodyNodes.front()->m_position.m_y = static_cast<int>(m_position.m_y);
}
#pragma endregion

#pragma region Destruction
void SceneObject::Denitialize()
{
	m_isActive = false;

	sp_objectManager->RemoveFromSceneObjects(this);
}
#pragma endregion